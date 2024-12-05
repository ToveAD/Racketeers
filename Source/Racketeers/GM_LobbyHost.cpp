// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_LobbyHost.h"
#include "AdvancedSessionsLibrary.h"
#include "GS_Lobby.h"
#include "LobbySpawnPoint.h"
#include "PC_Lobby.h"
#include "PS_Lobby.h"
#include "Kismet/GameplayStatics.h"

void AGM_LobbyHost::BeginPlay()
{
	Super::BeginPlay();
	SetUpSpawnPositions();
}

void AGM_LobbyHost::OnPostLogin(AController* NewPlayer)
{
	if (APC_Lobby* LobbyPC = Cast<APC_Lobby>(NewPlayer))
	{
		// Call the client RPC on the specific PlayerController
		LobbyPC->Client_ShowTeamSelectionWidget();
		
		Players.Add(LobbyPC);
		UpdatePlayers();
	}
}

/*void AGM_LobbyHost::OnLogout(AController* Exiting)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Left");
	
	if (APC_Lobby* LobbyPC = Cast<APC_Lobby>(Exiting))
	{
		Players.Remove(LobbyPC);
		UpdatePlayers();
	}
	
	UpdatePlayers();
	UpdateIfTeamFull();
}*/

void AGM_LobbyHost::SetUpSpawnPositions()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbySpawnPoint::StaticClass(), SpawnPositions);

	SpawnPositions.Sort([](const AActor& A, const AActor& B)
	{
		const ALobbySpawnPoint* SpawnPointA = Cast<ALobbySpawnPoint>(&A);
		const ALobbySpawnPoint* SpawnPointB = Cast<ALobbySpawnPoint>(&B);

		if (SpawnPointA && SpawnPointB)
		{
			return SpawnPointA->TeamID < SpawnPointB->TeamID;
		}
		return false;
	});
}

// Spawn the player at the first available spawn point and set spawn point in player controller
void AGM_LobbyHost::SpawnPlayer(APlayerController* PC, ETeams Team)
{
	if (APC_Lobby* PlayerController = Cast<APC_Lobby>(PC))
	{
		// Find the first available spawn point for the team
		for (const auto SP : SpawnPositions)
		{
			ALobbySpawnPoint* CurrentSP = Cast<ALobbySpawnPoint>(SP);

			// Check if the spawn point is available and the player is on the correct team
			if (CurrentSP->Team == Team && CurrentSP->PlayerController == nullptr)
			{
				// Set the player's new info
				APS_Lobby* PS = Cast<APS_Lobby>(PlayerController->PlayerState);
				PS->LobbyInfo.Team = Team;
				PS->LobbyInfo.PlayerName = PS->GetPlayerName();
				PS->LobbyInfo.SpawnPoint = CurrentSP;
				PS->LobbyInfo.TeamID = CurrentSP->TeamID;

				PlayerController->SpawnPoint = CurrentSP;
				CurrentSP->SetPlayerController(PlayerController);
				CurrentSP->Server_SpawnPlayer();

				UpdatePlayers();
				break;
			}
		}

		UpdateIfTeamFull();
	}
}

void AGM_LobbyHost::RemovePlayer(APlayerController* PC)
{
	if(ALobbySpawnPoint* SpawnPoint = Cast<ALobbySpawnPoint>(Cast<APC_Lobby>(PC)->SpawnPoint))
	{
		SpawnPoint->Server_RemovePlayer();
		Cast<APS_Lobby>(PC->PlayerState)->LobbyInfo.bIsReady = false;
		
		UpdatePlayers();
		UpdateIfTeamFull();
		UpdateIfEnoughPlayersToStart();
	}
}


void AGM_LobbyHost::UpdatePlayers()
{
	// Iterate through all spawn points and update the player info
	for (const auto SP : SpawnPositions)
	{
		ALobbySpawnPoint* CurrentSP = Cast<ALobbySpawnPoint>(SP);

		// Update the player info for all occupied spawn points
		if (CurrentSP && CurrentSP->PlayerController && Players.Contains(CurrentSP->PlayerController))
		{
			APS_Lobby* PS = Cast<APS_Lobby>(CurrentSP->PlayerController->PlayerState);
			CurrentSP->Multicast_UpdateWidgetInfo(PS->LobbyInfo, PS);
		}
	}

	UpdateIfTeamFull();
	UpdateIfEnoughPlayersToStart();
}

void AGM_LobbyHost::UpdateIfTeamFull()
{
	int NumPandaPlayers = 0;
	int NumRaccoonPlayers = 0;

	for (const auto SP : SpawnPositions)
	{
		if (const ALobbySpawnPoint* SpawnPoint = Cast<ALobbySpawnPoint>(SP))
		{
			if (SpawnPoint->PlayerController)
			{
				if (SpawnPoint->Team == ETeams::Team_Panda)
				{
					NumPandaPlayers++;
				}
				else if (SpawnPoint->Team == ETeams::Team_Raccoon)
				{
					NumRaccoonPlayers++;
				}
			}
		}
	}

	// Update the game state
	if (AGS_Lobby* GS = GetGameState<AGS_Lobby>())
	{
		GS->bPandaFull = NumPandaPlayers >= 3;
		GS->bRaccoonFull = NumRaccoonPlayers >= 3;
	}
}

void AGM_LobbyHost::UpdateIfEnoughPlayersToStart() const
{
	int NumOfReadyPlayers = 0;

	AGS_Lobby* GS = GetGameState<AGS_Lobby>();

	for(const auto player : GS->PlayerArray)
	{
		if(APS_Lobby* PS = Cast<APS_Lobby>(player))
		{
			if(PS->LobbyInfo.bIsReady)
			{
				NumOfReadyPlayers++;
			}
		}
	}
	
	GS->bEnoughPlayersToStart = (NumOfReadyPlayers == GS->PlayerArray.Num());
}

void AGM_LobbyHost::StartTheMatch()
{
	ProcessServerTravel(MapName);

	for(const auto player : Players)
	{
		if(APS_Lobby* PS = Cast<APS_Lobby>(player))
		{
			APC_Lobby* PC = Cast<APC_Lobby>(PS->GetOwner());
			PC->Client_OnStartMatch();
		}
	}
}


