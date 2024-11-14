// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersController.h"
#include <string>

#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "RacketeersGameStateBase.h"
#include "RacketeersGMBase.h"
#include "WidgetSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"



void ARacketeersController::Call_Interact_Implementation(const FString &string)
{
	//Change Player Enum Trough out Servers and send it
	UE_LOG(LogTemp, Warning, TEXT("Call_Interact_Implementation: %ls"), *string);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *string);
}

bool ARacketeersController::Call_Interact_Validate(const FString &string)
{
	return true;
}

void ARacketeersController::AddToWood_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Add Wood in ARacketeersController"));
		return;
	}
	State->AddToWood(Amount, Team);
}

bool ARacketeersController::AddToWood_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::AddToFiber_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Add Fiber in ARacketeersController"));
		return;
	}
	State->AddToFiber(Amount, Team);
}

bool ARacketeersController::AddToFiber_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::AddToMetal_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Add Metal in ARacketeersController"));
		return;
	}
	State->AddToMetal(Amount, Team);
}

bool ARacketeersController::AddToMetal_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::RemoveWood_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Remove Wood in ARacketeersController"));
		return;
	}
	State->RemoveWood(Amount, Team);
}

bool ARacketeersController::RemoveWood_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::RemoveFiber_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Remove Fiber in ARacketeersController"));
		return;
	}
	State->RemoveFiber(Amount, Team);
}

bool ARacketeersController::RemoveFiber_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::RemoveMetal_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Remove Metal in ARacketeersController"));
		return;
	}
	State->RemoveMetal(Amount, Team);
}

bool ARacketeersController::RemoveMetal_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::DamageBoat_Implementation(int Amount, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not DamageBoat in ARacketeersController"));
		return;
	}
	State->DamageBoat(Amount, Team);
}

bool ARacketeersController::DamageBoat_Validate(int Amount, ETeams Team)
{
	return true;
}

void ARacketeersController::ActivateWidget_Implementation(FName Name, UUserWidget* Widget)
{
	int32 s = GetUniqueID();
	FString String = FString::FromInt(s);

	bhavePressedContinue = false;
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *String);
	if(Widget == nullptr)
	{
		return;
	}
	
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(WS == nullptr)
	{
		return;
	}
	if(!WS->WidgetComponents.Contains(Name))
	{
		Widget->AddToViewport();
		WS->WidgetComponents.Add(Name, Widget);
	}
	//UserWidget = Widget;
	
}

void ARacketeersController::RemoveWidget_Implementation(FName Name)
{
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(WS == nullptr)
	{
		return;
	}
	UUserWidget* Widget= *WS->WidgetComponents.Find(Name);
	if(Widget == nullptr)
	{
		return;
	}
	Widget->RemoveFromParent();
	if(WS->WidgetComponents.Contains(Name))
	{
		WS->WidgetComponents.Remove(Name);
	}
	bhavePressedContinue = false;

	//UserWidget->RemoveFromParent();
	
}

void ARacketeersController::RequestRemoveWidget_Implementation()
{
	if(bhavePressedContinue)
	{
		return;
	}
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(WS == nullptr)
	{
		return;
	}
	if(WS->WidgetComponents.Num() == 0)
	{
		return;
	}
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(State == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not DamageBoat in ARacketeersController"));
		return;
	}
	bhavePressedContinue = true;
	State->RequestToRemoveWidget();
}

bool ARacketeersController::RequestRemoveWidget_Validate()
{
	return true;
}

void ARacketeersController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARacketeersController, bhavePressedContinue);
	
}

void ARacketeersController::RemoveResource_Implementation(int Amount, EResources Resource, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (State == nullptr)
	{
		return;
	}
	State->RemoveResource(Amount, Resource, Team);
}

bool ARacketeersController::RemoveResource_Validate(int Amount, EResources Resource, ETeams Team)
{
	return true;
}

void ARacketeersController::AddResource_Implementation(int Amount, EResources Resource, ETeams Team)
{
	ARacketeersGameStateBase* State = Cast<ARacketeersGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (State == nullptr)
	{
		return;
	}
	State->AddResource(Amount, Resource, Team);
}

bool ARacketeersController::AddResource_Validate(int Amount, EResources Resource, ETeams Team)
{
	return true;
}
