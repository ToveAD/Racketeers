// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGMBase.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
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
	Phase_1->TimeLimit = 15.0f;
	Phase_1->LevelToLoad = "Phase1_GamePlay";
	Phase_1->StartPhaseName = "P1";
	
	Phase_2->State = FPhaseState::Phase_2;
	Phase_2->TimeLimit = 15.0f;
	Phase_2->LevelToLoad = "Phase2_GamePlay";
	Phase_2->StartPhaseName = "P2";
	
	Phase_3->State = FPhaseState::Phase_3;
	Phase_3->TimeLimit = 15.0f;
	Phase_3->LevelToLoad = "Phase3_GamePlay";
	Phase_3->StartPhaseName = "P3";

	Phases.Push(Phase_1);
	Phases.Push(Phase_2);
	Phases.Push(Phase_3);
	//Set Current Phase
	CurrentPhase = Phases[0];

	CurrentTime = 0;
	//UGameplayStatics::GetStreamingLevel(GetWorld(), (TEXT("%s"), *Phases[GetNextPhaseNumber()]->LevelToLoad))->SetShouldBeLoaded(true);
	
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
		UE_LOG(LogTemp, Warning, TEXT("Restarting Game Phase"));
	
		Condition();
		Transition();
		
	}else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), CurrentTime);
		CurrentTime += DeltaSeconds;
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
	FLatentActionInfo ActionInfo;
	ActionInfo.Linkage = 0;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = TEXT("LoadLevel");
	ActionInfo.UUID = GetUniqueID();	

	UE_LOG(LogTemp, Warning, TEXT("Unload Level"))
	UnloadLevel((TEXT("%s"), *CurrentPhase->LevelToLoad), ActionInfo);
}

int ARacketeersGMBase::GetNextPhaseNumber()
{
	if(CurrentPhase->State == FPhaseState::Phase_3)
	{
		return 0;
	}
	else
	{
		return CurrentPhase->State+1;
	}
}

void ARacketeersGMBase::UnloadLevel(FName name, FLatentActionInfo& ActionInfo)
{
	UGameplayStatics::UnloadStreamLevel(GetWorld(), name, ActionInfo, false);
}

void ARacketeersGMBase::LoadLevel()
{
	FLatentActionInfo LoadActionInfo;
	
	LoadActionInfo.Linkage = 0;
	LoadActionInfo.CallbackTarget = this;
	LoadActionInfo.ExecutionFunction = TEXT("RespawnPlayers");
	LoadActionInfo.UUID = GetUniqueID();
	
	UGameplayStatics::LoadStreamLevel(GetWorld(), *Phases[GetNextPhaseNumber()]->LevelToLoad, true , false, LoadActionInfo);
}

void ARacketeersGMBase::RespawnPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("RespawnPlayers"));
	for (APlayerState* Player : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		AActor* PlayerStart = FindPlayerStart(Player->GetPlayerController(), Phases[GetNextPhaseNumber()]->StartPhaseName);
		Player->GetPawn()->SetActorLocation(PlayerStart->GetActorLocation());
	}
	SwitchState();
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
