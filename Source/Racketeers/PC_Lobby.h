// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
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


	// Show the team selection widget
	UFUNCTION(Client, Reliable)
	void Client_ShowTeamSelectionWidget();

	UFUNCTION()
	void RequestTeamSelection();
	//----------------------------------------------------------------------------------------------

	// Set the team on server
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetTeam(ETeams Team);

	UFUNCTION(BlueprintCallable)
	void SetTeam(ETeams Team);
	
};
