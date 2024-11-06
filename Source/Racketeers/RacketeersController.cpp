// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketeersController.h"

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