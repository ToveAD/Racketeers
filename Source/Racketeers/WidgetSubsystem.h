// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Engine/GameInstance.h"
#include "WidgetSubsystem.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadWidgetTest, FName, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnloadTest, FName, Name);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamage, float, Amount, ETeams, Team, APlayerController*, Controller = nullptr);
/**
 * Subsystem to Load Widgets 
 */
UCLASS()
class RACKETEERS_API UWidgetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/*
	 *Delegate called when something is picked upp 
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPickUp OnPickUp;
	/*
	 * Delegate Called When a Player/Boat has taken damage
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDamaged OnDamaged;
	/*
	 * Delegate Called When Someone has taken damage
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTakeDamage OnTakeDamage;

	

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
	TMap<FName, UUserWidget*> ActiveWidgetComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UUserWidget*> MapOfAllPlayerWidget;

	UFUNCTION(Server, Reliable, WithValidation)
	void IncrementPlayersPressed();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetCanRunTick(bool b);

	bool CanRun = true;

	int32 CountOfPlayersPressed;
	
};
