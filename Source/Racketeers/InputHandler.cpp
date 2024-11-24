// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHandler.h"

#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UInputHandler::UInputHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInputHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInputHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInputHandler::SwitchInputMapping(bool bIsReversed, UInputMappingContext* InputOne, UInputMappingContext* InputTwo, APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			if (InputSubsystem && InputOne && InputTwo)
			{
				if (bIsReversed)
				{
					InputSubsystem->RemoveMappingContext(InputTwo);
					InputSubsystem->AddMappingContext(InputOne, 1);
					UE_LOG(LogTemp, Log, TEXT("Switched to boat input mapping context: IMC_Boat"));
				}
				else
				{
					InputSubsystem->RemoveMappingContext(InputOne);
					InputSubsystem->AddMappingContext(InputTwo, 1);
					UE_LOG(LogTemp, Log, TEXT("Switched to Player input mapping context: IMC_Default"));
				}
			}
		}
	}
}

