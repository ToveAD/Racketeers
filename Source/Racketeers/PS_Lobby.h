// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_Base.h"
#include "GameModeStructs.h"
#include "LobbySpawnPoint.h"
#include "PS_Lobby.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "LobbyInfo")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "LobbyInfo")
	ETeams Team;

	UPROPERTY(BlueprintReadWrite, Category = "LobbyInfo")
	int TeamID;

	UPROPERTY(BlueprintReadWrite, Category = "LobbyInfo")
	bool bIsReady;

	UPROPERTY(BlueprintReadWrite, Category = "LobbyInfo")
	ALobbySpawnPoint* SpawnPoint;

	FLobbyInfo()
	{
		PlayerName = "";
		Team = ETeams::NONE;
		TeamID = -1;
		bIsReady = false;
		SpawnPoint = nullptr;
	}
};



UCLASS()
class RACKETEERS_API APS_Lobby : public APS_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	FLobbyInfo LobbyInfo;

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
