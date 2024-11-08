// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase.h"

UPhase::UPhase()
{
	PrimaryComponentTick.bCanEverTick = false;
	TimeLimit = 0;
	State = Phase_1;
}

UPhase::UPhase(float P_TimeLimit, FPhaseState P_State)
{
	TimeLimit = P_TimeLimit;
	State = P_State;
	
}


void UPhase::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPhase::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}




bool UPhase::LoseCondition(FTeam TeamA, FTeam TeamB)
{
	
	
	return true;
}
bool UPhase::WinCondition(FTeam TeamA, FTeam TeamB)
{

	
	return true;
}

void UPhase::ConditionExecutuion()
{
	
}



