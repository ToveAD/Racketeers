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

	UFUNCTION(BlueprintCallable)
	void ChangeCurrentPhase(TEnumAsByte<EPhaseState> NewPhase);

	UFUNCTION(BlueprintCallable)
	int32 GetTeamResources(ETeams Team, EResources Resource) const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetMaxHealth(ETeams Team, int32 MaxHealth);
	// -- End of Methods

	UFUNCTION(BlueprintCallable)
	void DamageBoat(int Amount, ETeams Team);
	
	UFUNCTION(BlueprintCallable)
	void RequestToRemoveWidget();
	
	UPROPERTY(ReplicatedUsing=OnRep_PickUp, BlueprintReadWrite)
	FResources RacconResource;
	UPROPERTY(ReplicatedUsing=OnRep_PickUp, BlueprintReadWrite)
	FResources RedPandasResource;
	UPROPERTY(ReplicatedUsing=OnRep_PhaseChange, BlueprintReadWrite)
	TEnumAsByte<EPhaseState> CurrentPhase;
	UFUNCTION(BlueprintCallable)
	void OnRep_PickUp();

	UFUNCTION(BlueprintCallable)
	void OnRep_PhaseChange();
	
	UFUNCTION(BlueprintCallable)
	void SetRandomNumber(int Number);
	
	
	UFUNCTION(BlueprintCallable)
	void AddResource(int Amount, EResources Resource, ETeams Team);
	UFUNCTION(BlueprintCallable)
	void RemoveResource(int Amount, EResources Resource, ETeams Team);


	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsMetal;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RacconsRoundsWon;


	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float RacconsMaxHealth;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	float RacconsBoatHealth;  // - repnotify
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasWood;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasFiber;
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 RedPandasMetal;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	int32 RedPandasRoundsWon;

	UFUNCTION(BlueprintCallable)
	void OnRep_HealthChanged();

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	float RedPandasMaxHealth;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged, BlueprintReadWrite)
	float RedPandasBoatHealth; // - repnotify
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	ETeams GameWinner = ETeams::NONE;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	float Phase2RandomNumber; 


private:
	void CheckOnRepHealthChanged();

	
};

