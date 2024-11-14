// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Engine/GameInstance.h"
#include "WidgetSubsystem.generated.h"

/**
 * Subsystem to Load Widgets 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadWidgetTest, FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnloadTest, FName, Name);

//Add So the key bind is instead binded with a Enhanced Input Action to a specifik function
UCLASS()
class RACKETEERS_API UWidgetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	/*
	 * Delegate To load a Widget onto the screen and to the WidgetComponents
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadWidgetTest OnLoadWidget;

	/*
	 * Delegate To Unload a widget by name
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnUnloadTest OnUnload;


	/* Array with the local players active widgets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UUserWidget*> WidgetComponents;

	UFUNCTION(Server, Reliable, WithValidation)
	void IncrementPlayersPressed();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetCanRunTick(bool b);

	bool CanRun = true;

	int32 CountOfPlayersPressed;
	
};
