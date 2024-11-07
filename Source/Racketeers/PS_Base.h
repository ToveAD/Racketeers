// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PS_Base.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API APS_Base : public APlayerState
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	bool bIsReady;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	APlayerState* PlayerState;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	ETeams Team;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerInfo")
	int TeamPlayerID;
	
};
