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
	
	UFUNCTION(BlueprintCallable)
	void SetMaterial(const FResources& Materials, Teams team);
	UFUNCTION(BlueprintCallable)
	FResources GetResources(Teams team);

	FResources Team_A_Materials;
	FResources Team_B_Materials;

	
};
