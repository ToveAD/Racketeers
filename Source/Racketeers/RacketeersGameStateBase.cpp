// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersGameStateBase.h"

#include <filesystem>

#include "BaseGameInstance.h"
#include "GameplayTagContainer.h"
#include "RacketeersGMBase.h"
#include "WidgetSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class UWidgetSubsystem;

void ARacketeersGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARacketeersGameStateBase, RacconResource);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasResource);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsRoundsWon);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsBoatHealth);
	DOREPLIFETIME(ARacketeersGameStateBase, RacconsMaxHealth);

	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasWood);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasFiber);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasMetal);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasRoundsWon);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasBoatHealth);
	DOREPLIFETIME(ARacketeersGameStateBase, RedPandasMaxHealth);

	DOREPLIFETIME(ARacketeersGameStateBase, GameWinner);
	
	DOREPLIFETIME(ARacketeersGameStateBase, CurrentPhase);
	
	DOREPLIFETIME(ARacketeersGameStateBase, Phase2RandomNumber);
}


void ARacketeersGameStateBase::BeginPlay()
{
	Super::BeginPlay();


	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
	if (GI->CheckIfDataToTransfer())
	{
		FGameStatsPackage Package = GI->GetDataTransferPackage();

		RacconsWood = Package.RacconsWood;
		RacconsFiber = Package.RacconsFiber;
		RacconsMetal = Package.RacconsMetal;
		RacconsRoundsWon = Package.RacconsRoundsWon;
		RacconsBoatHealth = Package.RacconsBoatHealth;
		RedPandasWood = Package.RedPandasWood;
		RedPandasFiber = Package.RedPandasFiber;
		RedPandasMetal = Package.RedPandasMetal;
		RedPandasRoundsWon = Package.RedPandasRoundsWon;
		RedPandasBoatHealth = Package.RedPandasBoatHealth;
		GameWinner = Package.WonTeam;
		GI->ClearDataStatsPackage();
	}

	if (HasAuthority())
	{
		AddResource(3, EResources::WOOD, ETeams::Team_Racoon);
		AddResource(3, EResources::WOOD, ETeams::Team_Panda);
		AddResource(3, EResources::FIBER, ETeams::Team_Racoon);
		AddResource(3, EResources::FIBER, ETeams::Team_Panda);
		AddResource(3, EResources::METAL, ETeams::Team_Racoon);
		AddResource(3, EResources::METAL, ETeams::Team_Panda);
		SetMaxHealth(ETeams::Team_Racoon, RacconsMaxHealth);
		SetMaxHealth(ETeams::Team_Panda, RedPandasMaxHealth);
	}

}

void ARacketeersGameStateBase::AddToWood(int Amount, ETeams Team)
{
	if (Team == ETeams::Team_Racoon)
	{
		RacconsWood += Amount;
		return;
	}
	RedPandasWood += Amount;
}

void ARacketeersGameStateBase::AddToFiber(int Amount, ETeams Team)
{
	if (Team == ETeams::Team_Racoon)
	{
		RacconsFiber += Amount;
		return;
	}
	RedPandasFiber += Amount;
}

void ARacketeersGameStateBase::AddToMetal(int Amount, ETeams Team)
{
	if (Team == ETeams::Team_Racoon)
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

void ARacketeersGameStateBase::ChangeCurrentPhase(TEnumAsByte<EPhaseState> NewPhase)
{
	CurrentPhase = NewPhase;
	if(GetLocalRole() == ROLE_Authority)
	{
		OnRep_PhaseChange();
	}

}

int32 ARacketeersGameStateBase::GetTeamResources(ETeams Team, EResources Resource) const
{
	if(Team == ETeams::Team_Racoon)
	{
		int Space = (int)Resource;
		int32* material = (int32*)((&RacconResource.Wood + Space));
		int32 MaterialAmount = material[0];
		return MaterialAmount;
	}
	int Space = (int)Resource;
	int32* material = (int32*)((&RedPandasResource.Wood + Space));
	int32 MaterialAmount = material[0];
	return MaterialAmount;
}

void ARacketeersGameStateBase::SetMaxHealth_Implementation(ETeams Team, int32 MaxHealth)
{
	if(Team == ETeams::Team_Racoon)
	{
		RacconsMaxHealth = MaxHealth;
		return;
	}
	RedPandasMaxHealth = MaxHealth;
}


void ARacketeersGameStateBase::DamageBoat(int Amount, ETeams Team)
{
	ARacketeersGMBase* GM = Cast<ARacketeersGMBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ARacketeersGameStateBase::DamageBoat GM is equal to nullptr"));
		return;
	}

	if (Team == ETeams::Team_Racoon)
	{
		RacconsBoatHealth -= Amount;
		if (RacconsBoatHealth <= 0)
		{
			//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
			RedPandasRoundsWon++;
			GM->RoundCompletion();
		}
		return;
	}
	RedPandasBoatHealth -= Amount;
	if (RedPandasBoatHealth <= 0)
	{
		//call method in GameMode to set the victor and the score, either ending the game or go ti next phase based on what round the game is on
		RacconsRoundsWon++;
		GM->RoundCompletion();
	}
}


void ARacketeersGameStateBase::RequestToRemoveWidget()
{
	ARacketeersGMBase* GM = Cast<ARacketeersGMBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM == nullptr)
	{
		return;
	}
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if (WS == nullptr)
	{
		return;
	}
	WS->IncrementPlayersPressed();

	//GM->UnloadWidget();
}


void ARacketeersGameStateBase::OnRep_PickUp()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "PICKED UP");
	}
	GetGameInstance()->GetSubsystem<UWidgetSubsystem>()->OnPickUp.Broadcast();

}

void ARacketeersGameStateBase::OnRep_PhaseChange()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "PHASE CHANGE");
	}
}

void ARacketeersGameStateBase::SetRandomNumber(int Number)
{
	Phase2RandomNumber = Number;
}

void ARacketeersGameStateBase::AddResource(int Amount, EResources Resource, ETeams Team)
{
	if (Team == ETeams::Team_Racoon)
	{
		int Space = (int)Resource;
		int32* material = (int32*)((&RacconResource.Wood + Space));
		if (material == nullptr)
		{
			return;
		}
		material[0] += Amount;
		if(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetLocalRole()  == ENetRole::ROLE_Authority)
		{
			OnRep_PickUp();
		}

		return;
	}
	int Space = (int)Resource;
	int32* material = (int32*)((&RedPandasResource.Wood + Space));
	if (material == nullptr)
	{
		return;
	}
	material[0] += Amount;


	if(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		OnRep_PickUp();
	}

	
}

//Callas på clienten sen på servern
void ARacketeersGameStateBase::RemoveResource(int Amount, EResources Resource, ETeams Team)
{
	if (Team == ETeams::Team_Racoon)
	{
		int Space = (int)Resource;
		int32* material = (int32*)((&RacconResource.Wood + Space));
		if (material == nullptr)
		{
			return;
		}
		material[0] -= Amount;
		if (material[0] < 0)
		{
			material[0] = 0;
		}
		if(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetLocalRole() == ENetRole::ROLE_Authority)
		{
			OnRep_PickUp();
		}
		return;
	}

	int Space = (int)Resource;
	int32* material = (int32*)((&RedPandasResource.Wood + Space));
	if (material == nullptr)
	{
		return;
	}
	material[0] -= Amount;
	if (material[0] < 0)
	{
		material[0] = 0;
	}
	if(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetLocalRole()  == ENetRole::ROLE_Authority)
	{
		OnRep_PickUp();
	}

}
