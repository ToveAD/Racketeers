// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ARacketeersGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsBoatHealth);
	
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasBoatHealth);
}


void ARacketeersGameStateBase::AddToWood(int Amount, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		RacconsWood += Amount;
		return;
	}
	RedPandasWood += Amount;
	
}

void ARacketeersGameStateBase::AddToFiber(int Amount, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		RacconsFiber += Amount;
		return;
	}
	RedPandasFiber += Amount;
}

void ARacketeersGameStateBase::AddToMetal(int Amount, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		RacconsMetal += Amount;
		return;
	}
	RedPandasMetal += Amount;
}

void ARacketeersGameStateBase::RemoveWood(int Amount, Teams Team)
{
	AddToWood(-Amount, Team);
}

void ARacketeersGameStateBase::RemoveFiber(int Amount, Teams Team)
{
	AddToFiber(-Amount, Team);
}

void ARacketeersGameStateBase::RemoveMetal(int Amount, Teams Team)
{
	AddToMetal(-Amount, Team);
}


void ARacketeersGameStateBase::DamageBoat(int Amount, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		RacconsBoatHealth -= Amount;
		if(RacconsBoatHealth <= 0)
		{
			//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
		}
		return;
	}
	RedPandasBoatHealth -= Amount;
	if(RedPandasBoatHealth <= 0)
	{
		//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
	}
}


