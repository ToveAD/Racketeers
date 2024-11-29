// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_LobbyHost.h"
#include "AdvancedSessionsLibrary.h"
#include "AdvancedSteamFriendsLibrary.h"
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
	}
}


void AGM_LobbyHost::OnLogout(AController* Exiting)
{
	RemovePlayer(Cast<APC_Lobby>(Exiting));

}

void AGM_LobbyHost::SetUpSpawnPositions()
{
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALobbySpawnPoint::StaticClass(), TEXT("Panda"), PandaPositions);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALobbySpawnPoint::StaticClass(), TEXT("Raccoon"), RaccoonPositions);


	// Sort the spawn points by TeamID
	PandaPositions.Sort([](const AActor& A, const AActor& B)
	{
		const ALobbySpawnPoint* SpawnPointA = Cast<ALobbySpawnPoint>(&A);
		const ALobbySpawnPoint* SpawnPointB = Cast<ALobbySpawnPoint>(&B);

		if (SpawnPointA && SpawnPointB)
		{
			return SpawnPointA->TeamID < SpawnPointB->TeamID;
		}
		return false;
	});

	RaccoonPositions.Sort([](const AActor& A, const AActor& B)
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
    	
        // Handle the case where the player has a spawn point
        if (PlayerController->SpawnPoint != nullptr)
        {
        	
            // If the player is already on the team, return
            if (Cast<APS_Lobby>(PlayerController->PlayerState)->LobbyInfo.Team == Team)
            {
                return;
            }

            // If the player already has a spawn point on the other team, remove the player from the spawn point
            if (Cast<APS_Lobby>(PlayerController->PlayerState)->LobbyInfo.Team != Team)
            {
                RemovePlayer(PC);
            }
        }

    	// Find the first available spawn point for the team
        for (const TArray<AActor*>& SpawnPositions = (Team == ETeams::Team_Panda) ? PandaPositions : RaccoonPositions; const auto SP : SpawnPositions)
    	{
		    if (ALobbySpawnPoint* SpawnPoint = Cast<ALobbySpawnPoint>(SP); SpawnPoint && SpawnPoint->PlayerController == nullptr)
    		{
    			PlayerController->SpawnPoint = SpawnPoint;
    			SpawnPoint->SpawnPlayer(PlayerController, Team);

		    	APS_Lobby* PS = Cast<APS_Lobby>(PlayerController->PlayerState);
		    	
		    	PS->LobbyInfo.Team = Team;
		    	PS->LobbyInfo.PlayerName = PlayerController->PlayerState->GetPlayerName();
		    	
		    	// Update the player info in the widget for all players
		    	SpawnPoint->Multicast_UpdateWidgetInfo(PS->LobbyInfo);
		    	
				UpdateIfTeamFull();
    			return;
    		}
    	}
    }
}

UTexture2D* AGM_LobbyHost::GetSteamAvatar(APlayerController* PC)
{
	FBPUniqueNetId NetID;
	//UAdvancedSessionsLibrary::GetUniqueNetIDFromPlayerState(PC->PlayerState, NetID);
	//EBlueprintAsyncResultSwitch ResultSwitch;
	//UTexture2D* AvatarTexture = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(NetID, ResultSwitch, SteamAvatarSize::SteamAvatar_Medium);

	/*if(ResultSwitch == EBlueprintAsyncResultSwitch::OnSuccess)
	{
		return AvatarTexture;
	}
	else
	{
		return UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(NetID, ResultSwitch, SteamAvatarSize::SteamAvatar_Medium);
	}*/
	return nullptr;
}


void AGM_LobbyHost::RemovePlayer(APlayerController* PC)
{
	if (APC_Lobby* PlayerController = Cast<APC_Lobby>(PC))
	{
		if (PlayerController->SpawnPoint)
		{
			PlayerController->SpawnPoint->RemovePlayer();
			PlayerController->SpawnPoint = nullptr;
		}
	}
}

void AGM_LobbyHost::UpdatePlayerPositions(ETeams Team)
{

}

void AGM_LobbyHost::UpdateIfTeamFull()
{
	// Variables to track if teams are full
	bool bPandaTeamFull = true;
	bool bRaccoonTeamFull = true;

	// Check Panda team spawn points
	for (const auto SP : PandaPositions)
	{
		if (ALobbySpawnPoint* SpawnPoint = Cast<ALobbySpawnPoint>(SP))
		{
			if (SpawnPoint->PlayerController == nullptr) // Empty spawn point found
			{
				bPandaTeamFull = false;
				break;
			}
		}
	}

	// Check Raccoon team spawn points
	for (const auto SP : RaccoonPositions)
	{
		if (ALobbySpawnPoint* SpawnPoint = Cast<ALobbySpawnPoint>(SP))
		{
			if (SpawnPoint->PlayerController == nullptr) // Empty spawn point found
			{
				bRaccoonTeamFull = false;
				break;
			}
		}
	}

	// Update the game state
	if (AGS_Lobby* GS = GetGameState<AGS_Lobby>())
	{
		GS->bPandaFull = bPandaTeamFull;
		GS->bRaccoonFull = bRaccoonTeamFull;
	}
}
