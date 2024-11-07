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


	//Methods for adding and removing to gathering score
	UFUNCTION()
	void AddToWood(int Amount, Teams Team);

	UFUNCTION()
	void AddToFiber(int Amount, Teams Team);

	UFUNCTION()
	void AddToMetal(int Amount, Teams Team);

	UFUNCTION()
	void RemoveWood(int Amount, Teams Team);

	UFUNCTION()
	void RemoveFiber(int Amount, Teams Team);

	UFUNCTION()
	void RemoveMetal(int Amount, Teams Team);
	// -- End of Methods

	UFUNCTION()
	void DamageBoat(int Amount, Teams Team);

	

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsMetal;
	UPROPERTY(Replicated, BlueprintReadWrite)
	float RacconsBoatHealth; 
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasMetal;
	UPROPERTY(Replicated, BlueprintReadWrite)
	float RedPandasBoatHealth; 
	


	
};
