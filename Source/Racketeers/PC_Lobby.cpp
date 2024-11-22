// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"

#include "AdvancedSessions.h"
#include "GM_LobbyHost.h"
#include "OnlineSubsystem.h"
#include "PS_Lobby.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineIdentityInterface.h"


// Show the team selection widget
void APC_Lobby::Client_ShowTeamSelectionWidget_Implementation()
{
	// Remove the player from the team if they are already on one
	if (APS_Lobby* LobbyPlayerState = Cast<APS_Lobby>(PlayerState))
	{
		if (LobbyPlayerState->Team != ETeams::NONE)
		{
			LobbyPlayerState->Team = ETeams::NONE;
			Server_RemovePlayer(this);
		}
	}

	// Create the team selection widget
	if (TeamSelectionWidgetClass)
	{
		
		if (UUserWidget* TeamSelectionWidget = CreateWidget<UUserWidget>(this, TeamSelectionWidgetClass))
		{
			TeamSelectionWidget->AddToViewport();
			bShowMouseCursor = true; // Show the cursor for UI interaction
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void APC_Lobby::RequestTeamSelection()
{
	if (HasAuthority()) // Ensure this is called on the server
	{
		Client_ShowTeamSelectionWidget(); // Notify the client
	}
}

void APC_Lobby::Server_SetTeam_Implementation(ETeams Team)
{
	// Notify the GameMode to handle spawning at team spawn points
	if (AGM_LobbyHost* GameMode = Cast<AGM_LobbyHost>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SpawnPlayer(this, Team);
	}
}

void APC_Lobby::SetTeam(ETeams Team)
{
	if (LobbyWidgetClass)
	{
		
		if (UUserWidget* LobbyWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass))
		{
			LobbyWidget->AddToViewport();

			// Set the team on the player state
			Cast<APS_Lobby>(PlayerState)->Team = Team;
		}
	}
	
	Server_SetTeam(Team);
}

// Remove the player from the server
void APC_Lobby::Server_RemovePlayer_Implementation(APlayerController* PC)
{
	if (AGM_LobbyHost* GameMode = Cast<AGM_LobbyHost>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RemovePlayer(PC);
	}
}

/*FString APC_Lobby::GetSteamPlayerName(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return TEXT("Invalid PlayerController");
	}

	// Get the Online Subsystem
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			// Get the Unique Net ID from PlayerController
			FUniqueNetIdRepl UniqueNetId = PlayerController->PlayerState->GetUniqueId();
			if (UniqueNetId.IsValid())
			{
				// Get the player's Steam display name
				FString PlayerName = IdentityInterface->GetPlayerNickname(*UniqueNetId.GetUniqueNetId());
				return PlayerName.IsEmpty() ? TEXT("Unknown Steam Player") : PlayerName;
			}
		}
	}

	// Fallback to PlayerState name if Steam name isn't available
	return PlayerController->PlayerState ? PlayerController->PlayerState->GetPlayerName() : TEXT("Unknown Player");
} */

