// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM_Base.h"
#include "TimerInfo.h"
#include "TransitionComponent.h"
#include "WidgetSubsystem.h"
#include "RacketeersGMBase.generated.h"

/**
 * Varje Start måste ha rätt phase tag, sen kan det också behövas att de olika starts får ett id för spelare och team.
 *
 *
 * IDAG,
 *
 * FIXA KLART WIDGET, Se till att poängen räknas ihopp av alla spelare. 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnloadWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnloadingMap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnloadedMap);




#define MAXTOTALROUNDS 8

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
	 * Transitions scenes inom varje phase som vissar score, 
	 * Kunna Bestäma I lobby om olika settings i lobby om vad det ska vara  best av 3 eller 5. 
	 *
	 *
	 *			 
	 */

public:

	ARacketeersGMBase();
	
	//Events / Delegates



	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	ATimerInfo* TimerInfo = nullptr;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	UTransitionComponent* TransitionComponent = nullptr;
	/*
	 * When A Widget Need To Load
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadWidget OnLoadWidget;
	/*
	* When A Widget Need To Unload
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadWidget OnUnloadWidget;
	/*
	* When the game mode has loaded a new map
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnloadedMap OnloadedMap;
	/*
	* When the game mode starts to unload a new map
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnUnloadingMap OnUnloadingMap;
	
	UWidgetSubsystem* WidgetSubsystem;
	
	UFUNCTION(BlueprintCallable)
	void UnloadWidget();

	int8 UnloadWidgetCount;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Respawn();
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void SpawnTeams();


	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Blueprintable, Category = "Phases")
	UPhase* Phase_1;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Blueprintable, Category = "Phases")
	UPhase* Phase_2;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Blueprintable, Category = "Phases")
	UPhase* Phase_3;

	UFUNCTION(BlueprintCallable)
	void LoadLevel();
	UFUNCTION(BlueprintCallable)
	void UnloadLevel(FName name, FLatentActionInfo& ActionInfo);
	
	UFUNCTION(BlueprintCallable)
	void RespawnPlayers();

	UFUNCTION(BlueprintCallable)
	void IncreaseTotalRounds();
	UFUNCTION(BlueprintCallable)
	void DecreaseTotalRounds();
	UFUNCTION(BlueprintCallable)
	void RoundCompletion();
	void BroadcastOnPlayerPressed();
	void IncrementPlayerCounter();
	int8 GetTotalRounds();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPhase* CurrentPhase;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UPhase*> Phases;

	

private:

	UPROPERTY(EditAnywhere)
	int8 TotalRounds;
	float CurrentTime;

	int GetNextPhaseNumber();
	
	//methods for progressing trough phases

	bool CheckIfGameIsOver();
	bool LoadTransitionStats();
	bool CheckWinnerOfRound();
	bool EndGame();
	void SwitchState();
	void Transition();


	UFUNCTION()
	void AllStagesFinished();
	
};

