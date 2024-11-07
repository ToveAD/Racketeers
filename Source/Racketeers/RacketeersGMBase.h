// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM_Base.h"
#include "RacketeersGMBase.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API ARacketeersGMBase : public AGM_Base
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
	 *
	 *
	 *
	 *
	 *			
	 *
	 *
	 *
	 *
	 *			 
	 */

public:

	ARacketeersGMBase();

	UFUNCTION(BlueprintCallable)
	void SetMaterial(const FResources& Materials, ETeams team);
	UFUNCTION(BlueprintCallable)
	FResources GetResources(ETeams team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Respawn();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void SpawnTeams();


	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	
	UPROPERTY(BlueprintReadWrite)
	UPhase* Phase_1;
	UPROPERTY(BlueprintReadWrite)
	UPhase* Phase_2;
	UPROPERTY(BlueprintReadWrite)
	UPhase* Phase_3;
	
private:

	UPROPERTY()
	UPhase* CurrentPhase;
	
	UPROPERTY()
	TArray<UPhase*> Phases;
	

	float CurrentTime;
	FResources Team_A_Materials;
	FResources Team_B_Materials;

	class AGS_Base* GameState;


	int GetNextPhaseNumber();


	


	void SwitchState();
	void Transition();
	void Condition();
	
};
