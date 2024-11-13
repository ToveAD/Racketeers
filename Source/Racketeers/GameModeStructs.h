// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.generated.h"

USTRUCT(BlueprintType)
struct FResources
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	int32 Wood;
	UPROPERTY(BlueprintReadWrite)
	int32 Fiber;
	UPROPERTY(BlueprintReadWrite)
	int32 Metal;

	FResources()
		: Wood(0), Fiber(0), Metal(0) {}
};

USTRUCT(BlueprintType)
struct FTeam
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FResources Resources;
	UPROPERTY(BlueprintReadWrite)
	int32 RoundsWon;
	UPROPERTY(BlueprintReadWrite)
	float BoatHealth;
	UPROPERTY(BlueprintReadWrite)
	FString Name;

	FTeam()
	: Resources(), RoundsWon(0), BoatHealth(100.0f), Name(TEXT("Unnamed")) {}
};




UENUM(BlueprintType)
enum class ETeams : uint8
{
	Team_Racoon,
	Team_Panda,
	NONE,
};

UENUM(BlueprintType, Blueprintable)
enum class ETransitionStates : uint8
{
	IN_PHASE,
	IN_TRANSITION_STATS,
};

USTRUCT(BlueprintType, Blueprintable)
struct FWidgetClassAndName
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget*  Widget;
};




