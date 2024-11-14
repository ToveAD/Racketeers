// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGMBase.h"

#include "BaseGameInstance.h"
#include "PS_Base.h"
#include "RacketeersGameStateBase.h"
#include "WidgetSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


//Göra ett event som en klass får som senare aktiverar widgeten.




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
 *	[GameMode] - Server Only
 *		
 *
 */

class UWidgetSubsystem;
class APS_Base;

ARacketeersGMBase::ARacketeersGMBase()
{
	UE_LOG(LogTemp, Warning, TEXT("AGM_Base::AGM_Base"));
}

void ARacketeersGMBase::UnloadWidget()
{

		UnloadWidgetCount++;
		if(UnloadWidgetCount == NumPlayers)
		{
			bIsGameActive = true;
			UnloadWidgetCount=0;
		}
	
	
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

	WidgetSubsystem = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();

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
	bIsGameActive = true;

	TotalRounds = 4;
}



void ARacketeersGMBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!WidgetSubsystem->CanRun)
	{
		return;
	}
	
	if(CurrentPhase == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Returned NullPtr"));
		return;
	}
	if(CurrentTime >= CurrentPhase->TimeLimit)
	{
		RoundCompletion();
	}else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Time: %f"), CurrentTime);
		CurrentTime += DeltaSeconds;
	}
}

void ARacketeersGMBase::RoundCompletion()
{
	//CheckIfGameIsOver
	//EndGame

	//CheckWinnerOfRound
	//LoadTransitionScreen (Eller en Level) - Show Scores, Everyone Press button to continue
		
	//Unload, Loads And Reset Players

	CurrentTime = 0;


	
	UE_LOG(LogTemp, Warning, TEXT("Check If Game Is Over"));
	if(CheckIfGameIsOver())
	{
		UE_LOG(LogTemp, Warning, TEXT("EndGame"));
		EndGame();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("CheckWinner Of Round"));
	CheckWinnerOfRound();

	UE_LOG(LogTemp, Warning, TEXT("Load Transition Stats"));
	LoadTransitionStats();

	UE_LOG(LogTemp, Warning, TEXT("Transition"));
	Transition();
}

bool ARacketeersGMBase::CheckWinnerOfRound()
{

	if(CurrentPhase->State == FPhaseState::Phase_3)
	{
		ARacketeersGameStateBase* GS = GetGameState<ARacketeersGameStateBase>();
		if(GS == nullptr) return false;
		if(GS->RacconsBoatHealth > GS->RedPandasBoatHealth)
		{
			GS->RacconsRoundsWon++;
			return true;
		}
		if(GS->RedPandasBoatHealth > GS->RacconsBoatHealth)
		{
			GS->RedPandasBoatHealth++;
			return true;
		}
		
		GS->RacconsRoundsWon++;
		GS->RedPandasRoundsWon++;
	}
	
	return true;
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

	OnUnloadingMap.Broadcast();
	
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



bool ARacketeersGMBase::CheckIfGameIsOver()
{

	ARacketeersGameStateBase* GS = this->GetGameState<ARacketeersGameStateBase>();
	
	if(CurrentPhase->State == FPhaseState::Phase_3)
	{
		int8 RoundsPlayed = GS->RacconsRoundsWon + GS->RedPandasRoundsWon;
		if(RoundsPlayed == GetTotalRounds() )
		{
			return true; 
		}
	}
	return false;
	
}

bool ARacketeersGMBase::LoadTransitionStats()
{
	
	//bIsGameActive = false;
	
	WidgetSubsystem->OnLoadWidget.Broadcast("ShowScore");
	WidgetSubsystem->SetCanRunTick(false);
	//OnLoadWidget.Broadcast();
	return true;
	
}

bool ARacketeersGMBase::EndGame()
{

	ARacketeersGameStateBase* GS = GetGameState<ARacketeersGameStateBase>();
	FGameStatsPackage Package{
		GS->RacconsWood,
		GS->RacconsFiber,
		GS->RacconsMetal,
		GS->RacconsRoundsWon,
		GS->RacconsBoatHealth,
		GS->RedPandasWood,
		GS->RedPandasFiber,
		GS->RedPandasMetal,
		GS->RedPandasRoundsWon,
		GS->RedPandasBoatHealth
	};
	if(Package.RacconsRoundsWon > Package.RedPandasRoundsWon)
	{
		Package.WonTeam = ETeams::Team_Racoon;
	}
	else if (Package.RacconsRoundsWon < Package.RedPandasRoundsWon)
	{
		Package.WonTeam = ETeams::Team_Racoon;
	}

	Package.WonTeam = ETeams::Team_Racoon;
	
	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	GI->SetDataToTransfer(Package);
	ProcessServerTravel("VictoryMap_GamePlay");
	return true;
}

void ARacketeersGMBase::IncreaseTotalRounds()
{
	if(TotalRounds + 1 > MAXTOTALROUNDS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached Max Total Rounds"));
		return;
	}
	TotalRounds++;
}

void ARacketeersGMBase::DecreaseTotalRounds()
{
	if(TotalRounds - 1 < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached under valid Total Rounds "));
		return;
	}
	TotalRounds--;
}

int8 ARacketeersGMBase::GetTotalRounds()
{
	return TotalRounds;
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
	OnloadedMap.Broadcast();
	for (int i = 0; i < this->GetGameState<AGameState>()->PlayerArray.Num(); ++i)
	{
		APS_Base* PS = Cast<APS_Base>(this->GetGameState<AGameState>()->PlayerArray[i]);
		FString TeamName;
		if(PS->PlayerInfo.Team == ETeams::Team_Racoon)
		{
			TeamName = "Team Racoons";
		}
		else
		{
			TeamName = "Team RedPandas";
		}
		TeamName.AppendInt(PS->PlayerInfo.TeamPlayerID);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TeamName);

		AActor* PlayerStart = FindPlayerStart(PS->GetPlayerController(),TeamName);

		AActor* PotentialParent = PS->GetPawn()->GetAttachParentActor();
		if( PotentialParent== nullptr)
		{
			EDetachmentRule InRule = EDetachmentRule();
			InRule = EDetachmentRule::KeepWorld;
			FDetachmentTransformRules DETCTMGR = FDetachmentTransformRules(InRule, true);
			PS->GetPawn()->DetachFromActor(DETCTMGR);
		}
		UE_LOG(LogTemp, Warning, TEXT("Player Name: %s"), *TeamName);
	
		PS->GetPawn()->SetActorLocation(PlayerStart->GetActorLocation());
	}
	/*
	for (APlayerState* Player : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		
		APS_Base* PS = Cast<APS_Base>(Player);
		FString TeamName;

		if(PS->PlayerInfo.Team == ETeams::Team_Racoon)
		{
			TeamName = "Team_Racoon";
		}
		else
		{
			TeamName = "Team_RedPandas";
		}
		TeamName.AppendInt(PS->PlayerInfo.TeamPlayerID);

		UE_LOG(LogTemp, Warning, TEXT("PLayer Name: %s"), *TeamName);
		AActor* PlayerStart = FindPlayerStart(Player->GetPlayerController(),TeamName);
		Player->GetPawn()->SetActorLocation(PlayerStart->GetActorLocation());
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("RespawnPlayers"));
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
