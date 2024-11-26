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
	Team_Raccoon,
	Team_Panda,
	NONE,
};

UENUM(BlueprintType)
enum class EResources : uint8
{
	WOOD = 0,
	FIBER = 1,
	METAL = 2
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
USTRUCT(BlueprintType)
struct FTimer
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Minutes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Seconds;
	
};

UENUM()
enum EGameStats
{
	PUSHES = 0,
	FALLS = 1,
	PARTCONSTRUCTED = 2,
	BOATDAMAGEDONE = 3,
	MISSES = 4
};

UENUM(BlueprintType)
enum EPart
{
	Hull_1,
	Hull_2,
	Cannon_1,
	Cannon_2,
	Sail_1,
	Sail_2,
};

USTRUCT(BlueprintType)
struct FParts
{
	GENERATED_BODY()
	EPart Hull;
	EPart Cannon;
	EPart Sail;
};

USTRUCT(BlueprintType)
struct FGameStats
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Pushes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Falls;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PartConstructed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 BoatDamageDone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Misses;
};





