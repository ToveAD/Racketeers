// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACKETEERS_API UInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Function to switch input mappings (for attaching/detaching control)
	UFUNCTION(BlueprintCallable, Category="Input")
	void SwitchInputMapping(bool IsReversed, UInputMappingContext* InputOne, UInputMappingContext* InputTwo, APlayerController* PlayerController);
};
