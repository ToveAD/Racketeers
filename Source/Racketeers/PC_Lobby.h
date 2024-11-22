// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "LobbySpawnPoint.h"
#include "GameFramework/PlayerController.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API APC_Lobby : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> TeamSelectionWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	ALobbySpawnPoint* SpawnPoint;


	// Show the team selection widget
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ShowTeamSelectionWidget();

	UFUNCTION()
	void RequestTeamSelection();
	
	//----------------------------------------------------------------------------------------------

	// Set the team on server
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetTeam(ETeams Team);

	// Set the team on client and spawn lobby UI
	UFUNCTION(BlueprintCallable)
	void SetTeam(ETeams Team);

	//----------------------------------------------------------------------------------------------

	// Remove the player from the spawn point on server
	UFUNCTION(Server, Reliable)
	void Server_RemovePlayer(APlayerController* PC);

	// ----------------------------------------------------------------------------------------------

	//UFUNCTION()
	//FString GetSteamPlayerName(APlayerController* PlayerController);

	
};
