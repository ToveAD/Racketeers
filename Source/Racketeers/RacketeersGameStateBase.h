// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "GS_Base.h"
#include "Phase.h"
#include "RacketeersGameStateBase.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseOneActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseTwoActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhaseThreeActive);

UCLASS()
class RACKETEERS_API ARacketeersGameStateBase : public AGS_Base
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPhaseOneActive OnPhaseOneActive;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPhaseTwoActive OnPhaseTwoActive;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPhaseThreeActive OnPhaseThreeActive;

	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ChangeCurrentPhase(TEnumAsByte<EPhaseState> NewPhase);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetMaxHealth(ETeams Team, int32 MaxHealth);
	UFUNCTION(BlueprintCallable)
	void DamageBoat(int Amount, ETeams Team);
	UFUNCTION(BlueprintCallable)
	void RequestToRemoveWidget();
	UFUNCTION(BlueprintCallable)
	void OnRep_PickUp();
	UFUNCTION(BlueprintCallable)
	void OnRep_PhaseChange();
	UFUNCTION(BlueprintCallable)
	void OnRep_HealthChanged();
	UFUNCTION(BlueprintCallable)
	void SetRandomNumber(int Number);
	UFUNCTION(BlueprintCallable)
	void AddResource(int Amount, EResources Resource, ETeams Team);
	UFUNCTION(BlueprintCallable)
	void AddToStats(int Amount, EGameStats Stat, ETeams Team);
	UFUNCTION(BlueprintCallable)
	void RemoveResource(int Amount, EResources Resource, ETeams Team);
	UFUNCTION(BlueprintCallable)
	int32 GetTeamResources(ETeams Team, EResources Resource) const;
	
	UFUNCTION(NetMulticast, Reliable)
	void AddPart(ETeams Team, EPart Part);
	UFUNCTION(NetMulticast, Reliable)
	void RemovePart(ETeams Team, EPart Part);
	
	UPROPERTY(ReplicatedUsing=OnRep_PickUp, BlueprintReadWrite)
	FResources RacconResource;
	UPROPERTY(ReplicatedUsing=OnRep_PickUp, BlueprintReadWrite)
	FResources RedPandasResource;
	UPROPERTY(ReplicatedUsing=OnRep_PhaseChange, BlueprintReadWrite)
	TEnumAsByte<EPhaseState> CurrentPhase;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FGameStats RaccoonsGameStats;
	UPROPERTY(Replicated, BlueprintReadWrite)
	FGameStats RedPandasGameStats;

	
	UPROPERTY(BlueprintReadWrite)
	TSet<TEnumAsByte<EPart>> RaccoonParts;
	UPROPERTY(BlueprintReadWrite)
	TSet<TEnumAsByte<EPart>> PandaParts; 


	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsRoundsWon;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float RaccoonsMaxHealth;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	float RaccoonsBoatHealth;  // - repnotify
	

	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	int32 RedPandasRoundsWon;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float RedPandasMaxHealth;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	float RedPandasBoatHealth; // - repnotify
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	ETeams GameWinner = ETeams::NONE;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	float Phase2RandomNumber; 


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 PandasReady;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 RaccoonsReady;

	
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 Phase3RandomNumber;
private:
	void CheckOnRepHealthChanged();
};

