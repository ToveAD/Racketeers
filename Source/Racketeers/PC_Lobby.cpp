// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"

#include "GM_LobbyHost.h"
#include "PS_Lobby.h"
#include "Blueprint/UserWidget.h"

void APC_Lobby::Client_ShowTeamSelectionWidget_Implementation()
{
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
	if (APawn* ControlledPawn = GetPawn())
	{
		// Set the team on the player state
		if (APS_Lobby* PS = Cast<APS_Lobby>(ControlledPawn->GetPlayerState()))
		{
			PS->Team = Team;
		}
	}
	
	// Notify the GameMode to handle spawning
	if (AGM_LobbyHost* GameMode = Cast<AGM_LobbyHost>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SpawnPlayer(Team);
	}
}

void APC_Lobby::SetTeam(ETeams Team)
{
	Server_SetTeam(Team);
}
