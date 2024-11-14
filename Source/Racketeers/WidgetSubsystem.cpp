// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSubsystem.h"

#include "GameFramework/GameStateBase.h"

void UWidgetSubsystem::IncrementPlayersPressed_Implementation()
{
	if(CanRun)
	{
		return;
	}

	
	CountOfPlayersPressed++;
	if(CountOfPlayersPressed == GetWorld()->GetGameState()->PlayerArray.Num())
	{
		TArray<FName> Names;
		WidgetComponents.GetKeys(Names);
		for (FName Name : Names)
		{
			OnUnload.Broadcast(Name);
		}
		CountOfPlayersPressed = 0;
		SetCanRunTick(true);
	}
}

bool UWidgetSubsystem::IncrementPlayersPressed_Validate()
{
	return true;
}

void UWidgetSubsystem::SetCanRunTick_Implementation(bool b)
{
	CanRun = b;
}

bool UWidgetSubsystem::SetCanRunTick_Validate(bool b)
{
	return true;
}
