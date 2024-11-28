// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"

#include "AdvancedSessions.h"
#include "GM_LobbyHost.h"
#include "OnlineSubsystem.h"
#include "PS_Lobby.h"
#include "Blueprint/UserWidget.h"

void APC_Lobby::BeginPlay()
{
	Super::BeginPlay();
}

// Show the team selection widget
void APC_Lobby::Client_ShowTeamSelectionWidget_Implementation()
{
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

void APC_Lobby::Client_ShowLobbyWidget_Implementation()
{
	if (LobbyWidgetClass)
	{
		
		if (UUserWidget* LobbyWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass))
		{
			LobbyWidget->AddToViewport();
		}
	}
}

void APC_Lobby::Client_ShowCosmeticWidget_Implementation()
{
  // Create the cosmetic selection widget
}

void APC_Lobby::Server_SpawnPlayer_Implementation(APlayerController* PC, ETeams Team)
{
	
	if (AGM_LobbyHost* GameMode = Cast<AGM_LobbyHost>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SpawnPlayer(PC, Team);
	}
	Cast<APC_Lobby>(PC)->Client_ShowLobbyWidget();
}

void APC_Lobby::Server_ToggleReady_Implementation()
{
	// Toggle the player's ready status on the GameMode
}











