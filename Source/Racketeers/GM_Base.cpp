// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Base.h"

#include "GS_Base.h"
#include "Kismet/GameplayStatics.h"


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

/*
AGM_Base::AGM_Base()
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::AGM_Base"));
}

void AGM_Base::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::Initiate Game"));
	
}


void AGM_Base::BeginPlay()
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

void AGM_Base::SetMaterial(const FResources& Materials, Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		Team_A_Materials = Materials;
	}
	Team_B_Materials = Materials;
	
}

FResources AGM_Base::GetResources(Teams Team)
{
	if(Team == Teams::TEAM_A)
	{
		return  Team_A_Materials;
	}
	return  Team_B_Materials;
	
}

void AGM_Base::Tick(float DeltaSeconds)
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
		RestartGame();
		UE_LOG(LogTemp, Display, TEXT("Restarting Game Phase"));
		/*
		Condition();
		SwitchState();
		Transition();
		
	}else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), CurrentTime);
		CurrentTime++;
	}
	
}
void AGM_Base::Condition()
{
	CurrentPhase->ConditionExecutuion();
}

void AGM_Base::SwitchState()
{
	if(CurrentPhase->State == FPhaseState::Phase_3)
	{
		CurrentPhase = Phases[0];
	}else
	{
		CurrentPhase = Phases[CurrentPhase->State+1];	
	}
}


void AGM_Base::Transition()
{
	//UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Phase2_GamePlay"));
	//Spawn the players to the positions
	//Load The Necassary 
}

void AGM_Base::Respawn_Implementation()
{
	//Respawn the player at valid spawn location
}

bool AGM_Base::Respawn_Validate()
{
	return true;
}

void AGM_Base::SpawnTeams_Implementation()
{
	RestartGame();
	//Spawn the entier team at there valid locations
}

bool AGM_Base::SpawnTeams_Validate()
{
	return true;
}

*/




