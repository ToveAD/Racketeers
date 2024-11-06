// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGMBase.h"


/*
 *	LoadIntoPhaseOne - (PlayerStart)
 *	Phase 1 completed
 *	Move Players To Phase2 level
 *
 *
 *	Servern (GameMode) Delar in De olika spelarna
	 *	SpawnPointSystem - Decides Where players Should Spawn
	 *	TeamSpawnPoints - The SpawnPoints To use
	 *	SpawnPoints - Get Local Player Move To The Location
 *
 *
 *
 *
 *
 *
 *	[GameMode] - Server Only
 *		
 *
 */

ARacketeersGMBase::ARacketeersGMBase()
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::AGM_Base"));
}

void ARacketeersGMBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::Initiate Game"));
	
}


void ARacketeersGMBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::BeginPlay"));
	
	//Set the GameState in GameMode
	//GameState = Cast<AGS_Base>(UGameplayStatics::GetGameState(GetWorld()));

	//Initilize variables
	Phase_1 = NewObject<UPhase>();
	Phase_2 = NewObject<UPhase>();
	Phase_3 = NewObject<UPhase>();
	
	
	//Declare the variables 
	Phase_1->State = FPhaseState::Phase_1;
	Phase_1->TimeLimit = 30.0f;
	Phase_2->State = FPhaseState::Phase_2;
	Phase_2->TimeLimit = 30.0f;
	Phase_3->State = FPhaseState::Phase_3;
	Phase_3->TimeLimit = 30.0f;

	Phases.Push(Phase_1);
	Phases.Push(Phase_2);
	Phases.Push(Phase_3);
	//Set Current Phase
	CurrentPhase = Phase_1;

	CurrentTime = 0;
	
}

void ARacketeersGMBase::SetMaterial(const FResources& Materials, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		Team_A_Materials = Materials;
	}
	Team_B_Materials = Materials;
	
}

FResources ARacketeersGMBase::GetResources(Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		return  Team_A_Materials;
	}
	return  Team_B_Materials;
	
}

void ARacketeersGMBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(CurrentPhase == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned NullPtr"));
		return;
	}
	if(CurrentTime >= CurrentPhase->TimeLimit)
	{
		CurrentTime = 0;
		//RestartGame();
		UE_LOG(LogTemp, Display, TEXT("Restarting Game Phase"));
	
		//Condition();
		//SwitchState();
		//Transition();
		
	}else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), CurrentTime);
		CurrentTime++;
	}
	
	
}
void ARacketeersGMBase::Condition()
{
	CurrentPhase->ConditionExecutuion();
}

void ARacketeersGMBase::SwitchState()
{
	if(CurrentPhase->State == FPhaseState::Phase_3)
	{
		CurrentPhase = Phases[0];
	}else
	{
		CurrentPhase = Phases[CurrentPhase->State+1];	
	}
}


void ARacketeersGMBase::Transition()
{
	//UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Phase2_GamePlay"));
	//Spawn the players to the positions
	//Load The Necassary 
}

void ARacketeersGMBase::Respawn_Implementation()
{
	//Respawn the player at valid spawn location
}

bool ARacketeersGMBase::Respawn_Validate()
{
	return true;
}

void ARacketeersGMBase::SpawnTeams_Implementation()
{
	RestartGame();
	//Spawn the entier team at there valid locations
}

bool ARacketeersGMBase::SpawnTeams_Validate()
{
	return true;
}
