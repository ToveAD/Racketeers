// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGameStateBase.h"

#include "RacketeersGMBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ARacketeersGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsRoundsWon);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsBoatHealth);
	
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasRoundsWon);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasBoatHealth);
}


void ARacketeersGameStateBase::AddToWood(int Amount, ETeams Team)
{
	if(Team == ETeams::Team_Racoon)
	{
		RacconsWood += Amount;
		return;
	}
	RedPandasWood += Amount;
	
}

void ARacketeersGameStateBase::AddToFiber(int Amount, ETeams Team)
{
	if(Team == ETeams::Team_Racoon)
	{
		RacconsFiber += Amount;
		return;
	}
	RedPandasFiber += Amount;
}

void ARacketeersGameStateBase::AddToMetal(int Amount, ETeams Team)
{
	if(Team == ETeams::Team_Racoon)
	{
		RacconsMetal += Amount;
		return;
	}
	RedPandasMetal += Amount;
}

void ARacketeersGameStateBase::RemoveWood(int Amount, ETeams Team)
{
	AddToWood(-Amount, Team);
}

void ARacketeersGameStateBase::RemoveFiber(int Amount, ETeams Team)
{
	AddToFiber(-Amount, Team);
}

void ARacketeersGameStateBase::RemoveMetal(int Amount, ETeams Team)
{
	AddToMetal(-Amount, Team);
}


void ARacketeersGameStateBase::DamageBoat(int Amount, ETeams Team)
{
	ARacketeersGMBase* GM = Cast<ARacketeersGMBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ARacketeersGameStateBase::DamageBoat GM is equal to nullptr"));
		return;
	}
	
	if(Team == ETeams::Team_Racoon)
	{
		RacconsBoatHealth -= Amount;
		if(RacconsBoatHealth <= 0)
		{
			//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
			GM->RoundCompletion();
		}
		return;
	}
	RedPandasBoatHealth -= Amount;
	if(RedPandasBoatHealth <= 0)
	{
		//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
		GM->RoundCompletion();
	}
}

