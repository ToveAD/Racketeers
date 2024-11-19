// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersController.h"
#include <string>

#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "RacketeersGameStateBase.h"
#include "RacketeersGMBase.h"
#include "WidgetSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"



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
	bhavePressedContinue = false;
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(!WS->ActiveWidgetComponents.Contains(Name) && WS->MapOfAllPlayerWidget.Contains(Name))
	{
		
		UUserWidget* W = *WS->MapOfAllPlayerWidget.Find(Name);
		if(W)
		{
		    W->AddToViewport();
			WS->ActiveWidgetComponents.Add(Name, W);
		}
	}
	//UserWidget = Widget;
	
	/*
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
	if(!WS->ActiveWidgetComponents.Contains(Name))
	{
		Widget->AddToViewport();
		WS->ActiveWidgetComponents.Add(Name, Widget);
	}
	//UserWidget = Widget;
	*/
}

void ARacketeersController::RemoveWidget_Implementation(FName Name)
{
	UWidgetSubsystem* WS = GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(WS == nullptr)
	{
		return;
	}
	if(WS->ActiveWidgetComponents.Contains(Name))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "IT DOES CONTAIN");
		UUserWidget* Widget= *WS->ActiveWidgetComponents.Find(Name);
		if(Widget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "REMOVE FROM PARENT");
			Widget->RemoveFromParent();
			WS->ActiveWidgetComponents.Remove(Name);
			bhavePressedContinue = false;
		}
	
	}


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
	if(WS->ActiveWidgetComponents.Num() == 0)
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

void ARacketeersController::BeginPlay()
{
	Super::BeginPlay();

	OnBeginPlayerEvent.Broadcast();
}

void ARacketeersController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ARacketeersController, bhavePressedContinue);
	
}

void ARacketeersController::ClientCheckReady_Implementation()
{
	if(bhavePressedContinue) return;
	bhavePressedContinue = true;
	ServerCheckReady();
}

bool ARacketeersController::ClientCheckReady_Validate()
{
	return true;
}

void ARacketeersController::ServerCheckReady_Implementation()
 {
	if(!HasAuthority()) return;
	ARacketeersGMBase* GM = Cast<ARacketeersGMBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "SERVER CHECK READY");
	GM->BroadcastOnPlayerPressed();
 }
 
 bool ARacketeersController::ServerCheckReady_Validate()
 {
	return true;
 }

void ARacketeersController::ServerMultiCastActivateTimer_Implementation()
{
	MultiCastActivateTimer(ATimerInfo::GetTime(), ATimerInfo::GetIsActive());

	for (APlayerState* PState : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		ARacketeersController* PController = Cast<ARacketeersController>(PState->GetPlayerController());
		
		PController->SetTimeSecondsn(ATimerInfo::GetTime(), ATimerInfo::GetIsActive());
	}
}

void ARacketeersController::MultiCastActivateTimer_Implementation(float T, bool SetIsActive)
{

	ATimerInfo::SetIsActive(SetIsActive);
	ATimerInfo::SetTime(T);
	SetTimeSecondsn(T, SetIsActive);
}

void ARacketeersController::SetTimeSecondsn_Implementation(float seconds, bool SetIsActive)
{
	
	ATimerInfo::SetTime(seconds);
	ATimerInfo::SetIsActive(SetIsActive);
}

void ARacketeersController::SetTime_Analog_Implementation(ATimerInfo* timer, int32 Minutes, int32 Seconds, bool SetIsActive)
{
	
}

void ARacketeersController::AddWidgetToViewport_Implementation(UUserWidget* Widget)
{
	if(Widget == nullptr)
	{
		return;
	}
	Widget->AddToViewport();
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

inline void ARacketeersController::SetServerTimeSeconds_Implementation(ARacketeersController* Controller ,float seconds, bool SetIsActive)
{
	ARacketeersGMBase* GM = Cast<ARacketeersGMBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GM == nullptr)
	{
		return;
	}
	Controller->SetTimeSecondsn(ATimerInfo::GetTime(), ATimerInfo::GetIsActive());
	
}

inline void ARacketeersController::SetServerTime_Analog_Implementation(ATimerInfo* timer ,int32 Minutes, int32 Seconds, bool SetIsActive)
{

}