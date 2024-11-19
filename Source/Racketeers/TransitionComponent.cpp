// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionComponent.h"

#include "RacketeersController.h"
#include "WidgetSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


class AGameState;
// Sets default values for this component's properties
UTransitionComponent::UTransitionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTransitionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetWidgetWithName(WidgetName);
	// ...
	
}

void UTransitionComponent::IncrementPlayerReady()
{
	CountPlayersReady++;
	if(bIsFinished && CountPlayersReady == UGameplayStatics::GetGameState(GetWorld())->PlayerArray.Num())
	{
		OnFinished.Broadcast();
	}
}


// Called every frame
void UTransitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTransitionComponent::SetWidgetWithName(FName Name)
{
	UWidgetSubsystem* WS = GetWorld()->GetGameInstance()->GetSubsystem<UWidgetSubsystem>();
	if(WS == nullptr)
	{
		return;
	}
	WidgetName = Name;
	//WS->ActiveWidgetComponents.Find(Name);
}

void UTransitionComponent::AddWidgetsToPlayers(AGameStateBase* GS)
{
	bIsFinished = false;
	bIsOn = true;
	OnBeginTransition.Broadcast();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "ADD WIDGETS TO PLAYERS");
	
	if(GS == nullptr)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "GAME STATE IS A NULLPTR");
		return;
	}
	for (APlayerState* PlayerState : GS->PlayerArray)
	{
	
		if(PlayerState == nullptr)
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "PLAYER STATE IS A NULLPTR");
			continue;
		}
		ARacketeersController* PC = Cast<ARacketeersController>(PlayerState->GetPlayerController());
		if (PC == nullptr)
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "PLAYER CONTROLLER STATE IS A NULLPTR");
			continue;
		}
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "CALL TO ACTIVATE WIDGET");
		PC->ActivateWidget(WidgetName, WidgetToDisplay);
	}
}

void UTransitionComponent::RemoveWidgetsFromPlayers()
{
	for (APlayerState*  PlayerState : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		ARacketeersController* PC = Cast<ARacketeersController>(PlayerState->GetPlayerController());
		if (PC == nullptr) continue;
		PC->RemoveWidget(WidgetName);
	}
}

void UTransitionComponent::LoadingFinished()
{
	bIsOn = false;
	bIsFinished = true;
	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	if(GS == nullptr) return;
	if(CountPlayersReady == GS->PlayerArray.Num())
	{
		OnFinished.Broadcast();
	}
}

