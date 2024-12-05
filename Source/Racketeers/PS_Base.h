// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "GameFramework/PlayerState.h"
#include "PS_Base.generated.h"

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

	FPlayerInfo()
	{
		PlayerController = nullptr;
		PlayerName = FText::FromString("Player");
		bIsReady = false;
		PlayerState = nullptr;
		Team = ETeams::NONE;
		TeamPlayerID = -1;
	}
	
};


UCLASS()
class RACKETEERS_API APS_Base : public APlayerState
{
	GENERATED_BODY()

public:


	APS_Base();

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "PlayerInfo")
	FPlayerInfo PlayerInfo;

	UFUNCTION(Server, Reliable, WithValidation,BlueprintCallable )
	void DamagePlayerBoat(APlayerState* PS,int Amount);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "PlayerInfo", EditAnywhere)
	float BoatHealth;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "PlayerInfo", EditAnywhere)
	float MaxBoatHealth;

	
	
};


