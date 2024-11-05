// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "GameFramework/GameMode.h"
#include "GM_Base.generated.h"

/**
 * 
 */




UCLASS()
class RACKETEERS_API AGM_Base : public AGameMode
{
	GENERATED_BODY()



	

	/*
	 *	GameMode should
	 *		- Gå igenom de olika faserna
	 *		- Keep states of the Game And What fase
	 *		- Keeping check on each teams Data
	 *
	 *
	 *
	 *		Funcitons:
	 *		void SwitchFase
	 *		void Lose
	 *		void Win
	 *
	 *		
	 *		
	 *		Varibles:
	 *		Struct - Wood, Fiber, Metal for TEAM A and TEAM B
	 *		Total Rounds
	 *		CurrentFase / FaseToSwitch
	 *		
	 */

public:
	UFUNCTION(BlueprintCallable)
	void SetMaterial(const FResources& Materials, Teams team);
	UFUNCTION(BlueprintCallable)
	FResources GetResources(Teams team);
private:
	



	
	FResources Team_A_Materials;

	FResources Team_B_Materials;
};
