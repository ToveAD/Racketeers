
#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Phase.Generated.h"

/*
	 *	Fases class
	 *
	 *
	 *	Phase 1
	 *		- Next Face condition is a time limit.
	 *	Phase 2
	 *		- Next Face Condition is a time limit.
	 *	Phase 3
	 *		- Determinig end of face is a timer
	 *		- lose condition is when the teams ship is destroyed or the team has less health than the other team
	 *		- win condition is when the other teams ship is destroyed or the team has more health than the other
	 *
	 */

enum FPhaseState
{
	Phase_1,
	Phase_2,
	Phase_3
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACKETEERS_API UPhase : public UActorComponent
{
public:
	GENERATED_BODY()
	 UPhase();
	 UPhase(float P_TimeLimit, FPhaseState P_State);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	float TimeLimit;
	
	FPhaseState State;
	FString LevelToLoad;
	FString StartPhaseName;

	//TArray<FTransform> SpawnsTeamA;
	//TArray<FTransform> SpawnsTeamB;
	
	//PlayerSpawns


	
	
};
