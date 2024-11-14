// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModeStructs.h"

#include "RacketeersController.generated.h"


/*
	 *	The Controller should
	 *		- Should get calls from clients to do an animation and sends that state to the other clients
	 *		-	- Collecting Resourcses probably activates an animation so that state changes
	 *		-	- Shooting from the boat.
	 *		-	-
	 *		- 
	 */


class UUserWidget;


UCLASS()
class RACKETEERS_API ARacketeersController : public APlayerController
{
	GENERATED_BODY()

	public:

	UPROPERTY(Replicated, EditAnywhere ,BlueprintReadWrite)
	bool bhavePressedContinue = false;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UUserWidget* UserWidget;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RequestRemoveWidget();
	
	//RPC to interact with a gatherable object
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Call_Interact(const FString &string);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddToWood(int Amount, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddToFiber(int Amount, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddToMetal(int Amount, ETeams Team);
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RemoveWood(int Amount, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RemoveFiber(int Amount, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RemoveMetal(int Amount, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void DamageBoat(int Amount, ETeams Team);

	UFUNCTION(CLient, Reliable, BlueprintCallable)
	void ActivateWidget(FName Name, UUserWidget* Widget);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void RemoveWidget(FName Name);


	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddResource(int Amount, EResources Resource, ETeams Team);
	
};
