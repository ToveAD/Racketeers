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
	Phase_1->TimeLimit = 10.0f;
	Phase_1->LevelToLoad = "Phase1_GamePlay";
	
	Phase_2->State = FPhaseState::Phase_2;
	Phase_2->TimeLimit = 10.0f;
	Phase_2->LevelToLoad = "Phase2_GamePlay";	
	
	Phase_3->State = FPhaseState::Phase_3;
	Phase_3->TimeLimit = 10.0f;
	Phase_3->LevelToLoad = "Phase3_GamePlay";	

	Phases.Push(Phase_1);
	Phases.Push(Phase_2);
	Phases.Push(Phase_3);
	//Set Current Phase
	CurrentPhase = Phases[0];

	CurrentTime = 0;
	
	ULevelStreaming* Level2 = UGameplayStatics::GetStreamingLevel(GetWorld(), TEXT("Phase2_GamePlay"));
	ULevelStreaming* Level3 = UGameplayStatics::GetStreamingLevel(GetWorld(), TEXT("Phase3_GamePlay"));
	Level2->SetShouldBeLoaded(false);
	Level3->SetShouldBeLoaded(false);
	UE_LOG(LogTemp, Warning, TEXT("Unload: %s"), *Level2->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Unload: %s"), *Level3->GetName());
	//UGameplayStatics::GetStreamingLevel(GetWorld(), (TEXT("%s"), *Phases[GetNextPhaseNumber()]->LevelToLoad))->SetShouldBeLoaded(true);
	
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
		UE_LOG(LogTemp, Warning, TEXT("Restarting Game Phase"));
	
		//Condition();
		Transition();
		SwitchState();
		
		
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
	FLatentActionInfo LoadActionInfo;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = TEXT("LoadLevel");
	ActionInfo.UUID = GetUniqueID();	
	
	//UGameplayStatics::UnloadStreamLevel(GetWorld(), (TEXT("%s"), *CurrentPhase->LevelToLoad))

	//Step 1 - Unload And Load
	//UGameplayStatics::GetStreamingLevel(GetWorld(), (TEXT("%s"), *Phases[GetNextPhaseNumber()]->LevelToLoad))->SetShouldBeLoaded(true);
	//UGameplayStatics::GetStreamingLevel(GetWorld(), (TEXT("%s"), *CurrentPhase->LevelToLoad))->SetShouldBeLoaded(false);

	UE_LOG(LogTemp, Warning, TEXT("Unload Level"));
	

	UnloadLevel((TEXT("%s"), *CurrentPhase->LevelToLoad), ActionInfo);
	
	LoadLevel((TEXT("%s"),  (TEXT("%s"), *Phases[GetNextPhaseNumber()]->LevelToLoad)), LoadActionInfo);

	//UGameplayStatics::LoadStreamLevel(GetWorld(),(TEXT("%s"), *Phases[GetNextPhaseNumber()]->LevelToLoad),true, false, ActionInfo);
	//Step 2 - Respawn the players

	
	UE_LOG(LogTemp, Warning, TEXT("Reset Players"));
	for(APlayerState* player : GetGameState<AGameStateBase>()->PlayerArray)
	{
		RestartPlayer(player->GetPlayerController());
	}
	
	
	//RestartGame();	
	

	//UGameplayStatics::LoadStreamLevel(GetWorld(), TEXT("Phase2_GamePlay"));
	//Spawn the players to the positions
	//Load The Necassary 
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

void ARacketeersGMBase::LoadLevel(FName name, FLatentActionInfo& ActionInfo)
{
	UGameplayStatics::LoadStreamLevel(GetWorld(), name, true , false, ActionInfo);
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
