// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "GS_Base.h"
#include "RacketeersGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API ARacketeersGameStateBase : public AGS_Base
{
	GENERATED_BODY()

	public:

	void BeginPlay() override;

	

	//Methods for adding and removing to gathering score
	UFUNCTION(BlueprintCallable)
	void AddToWood(int Amount, ETeams Team);

	UFUNCTION(BlueprintCallable)
	void AddToFiber(int Amount, ETeams Team);

	UFUNCTION(BlueprintCallable)
	void AddToMetal(int Amount, ETeams Team);

	UFUNCTION(BlueprintCallable)
	void RemoveWood(int Amount, ETeams Team);

	UFUNCTION(BlueprintCallable)
	void RemoveFiber(int Amount, ETeams Team);

	UFUNCTION(BlueprintCallable)
	void RemoveMetal(int Amount, ETeams Team);
	// -- End of Methods

	UFUNCTION(BlueprintCallable)
	void DamageBoat(int Amount, ETeams Team);


	UFUNCTION(BlueprintCallable)
	void RequestToRemoveWidget();

	

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsMetal;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsRoundsWon;
	UPROPERTY(Replicated, BlueprintReadWrite)
	float RacconsBoatHealth; 
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasMetal;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasRoundsWon;
	UPROPERTY(Replicated, BlueprintReadWrite)
	float RedPandasBoatHealth; 
	


	
};
