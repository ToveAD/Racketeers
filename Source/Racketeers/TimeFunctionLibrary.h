// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameModeStructs.h"
#include "TimeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API UTimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(Blueprintable, BlueprintCallable)
	static FTimer FloatToTimer(float Time);
	
};
