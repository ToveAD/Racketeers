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

		UpdatePlayers();
	}
}

void AGM_LobbyHost::OnLogout(AController* Exiting)
{
	if (APC_Lobby* LobbyPC = Cast<APC_Lobby>(Exiting))
	{
		if (ALobbySpawnPoint* SpawnPoint = LobbyPC->SpawnPoint)
		{
			SpawnPoint->SetPlayerController(nullptr);
		}
	}

	UpdatePlayers();
	UpdateIfTeamFull();
}

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
		// Handle the case where the player is already on the team
		if (PlayerController->SpawnPoint != nullptr && Cast<APS_Lobby>(PlayerController->PlayerState)->LobbyInfo.Team ==
			Team)
		{
			return;
		}

		// Remove the player from the previous spawn point
		if (PlayerController->SpawnPoint != nullptr)
		{
			PlayerController->SpawnPoint->SetPlayerController(nullptr);
		}

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

void AGM_LobbyHost::UpdatePlayers()
{
	// Iterate through all spawn points and update the player info
	for (const auto SP : SpawnPositions)
	{
		ALobbySpawnPoint* CurrentSP = Cast<ALobbySpawnPoint>(SP);

		// Update the player info for all occupied spawn points
		if (CurrentSP && CurrentSP->PlayerController)
		{
			APS_Lobby* PS = Cast<APS_Lobby>(CurrentSP->PlayerController->PlayerState);
			CurrentSP->Multicast_UpdateWidgetInfo(PS->LobbyInfo, PS);
		}

		// Remove the player info for all unoccupied spawn points
		if (CurrentSP && !CurrentSP->PlayerController)
		{
			CurrentSP->Server_RemovePlayer();
		}
	}
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

	for(const auto player : GetGameState<AGS_Lobby>()->PlayerArray)
	{
		if(APS_Lobby* PS = Cast<APS_Lobby>(player))
		{
			APC_Lobby* PC = Cast<APC_Lobby>(PS->GetOwner());
			PC->Client_OnStartMatch();
		}
	}
}


