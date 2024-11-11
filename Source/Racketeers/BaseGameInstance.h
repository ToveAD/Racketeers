// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "BaseGameInstance.generated.h"

/**
 * 
 */


enum ETransferState
{
	READY,
	EMPTY
};

USTRUCT(BlueprintType)
struct FGameStatsPackage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 RacconsWood = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RacconsFiber = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RacconsMetal = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RacconsRoundsWon = 0;
	UPROPERTY(BlueprintReadWrite)
	float RacconsBoatHealth = 0; 
	
	UPROPERTY(BlueprintReadWrite)
	int32 RedPandasWood = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RedPandasFiber = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RedPandasMetal = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 RedPandasRoundsWon = 0;
	UPROPERTY(BlueprintReadWrite)
	float RedPandasBoatHealth = 0;

	
	ETransferState State = EMPTY;
		
};



UCLASS()
class RACKETEERS_API UBaseGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()


public:
	UFUNCTION(Server, Reliable, WithValidation)
	void SetDataToTransfer(FGameStatsPackage GameStatsPackage);
	UFUNCTION(Server, Reliable, WithValidation)
	void SetGameStateData();
	UFUNCTION(Server, Reliable, WithValidation)
	void ClearDataStatsPackage();
	
private:
	FGameStatsPackage GetDataTransferPackage();
    	
	UPROPERTY()
	FGameStatsPackage TempSavedGameStatsPackage;
	


	
	
	

	
	
};
