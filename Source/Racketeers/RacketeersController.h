// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModeStructs.h"
#include "Net/UnrealNetwork.h"

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


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginPlayerEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPressedReady, ETeams, Team);

UCLASS()
class RACKETEERS_API ARacketeersController : public APlayerController
{
	GENERATED_BODY()

	

	public:
	UPROPERTY(EditAnywhere ,BlueprintReadWrite)
	bool bhavePressedContinue = false;
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, BlueprintCallable, BlueprintAssignable)
	FOnBeginPlayerEvent OnBeginPlayerEvent;
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, BlueprintCallable, BlueprintAssignable)
	FOnPlayerPressedReady OnPlayerPressedReady;
	UUserWidget* UserWidget;

	
	void BeginPlay() override;
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RequestRemoveWidget();
	
	//RPC to interact with a gatherable object
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Call_Interact(const FString &string);
	/*

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
	*/
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void DamageBoat(int Amount, ETeams Team);

	UFUNCTION(CLient, Reliable, BlueprintCallable)
	void ActivateWidget(FName Name, UUserWidget* Widget);

	UFUNCTION(CLient, Reliable, BlueprintCallable)
	void AddWidgetToViewport(UUserWidget* Widget);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void RemoveWidget(FName Name);

	UFUNCTION(Client, Reliable , BlueprintCallable, Blueprintable)
	void SetTimeSecondsn(float seconds, bool SetIsActive);
	UFUNCTION(Client, Reliable , BlueprintCallable, Blueprintable)
	void SetTime_Analog(ATimerInfo* timer ,int32 Minutes, int32 Seconds, bool SetIsActive);

	UFUNCTION(Server, Reliable , BlueprintCallable, Blueprintable)
	void SetServerTimeSeconds(ARacketeersController* Controller ,float seconds, bool SetIsActive);
	
	UFUNCTION(Server, Reliable , BlueprintCallable, Blueprintable)
	void SetServerTime_Analog(ATimerInfo* timer ,int32 Minutes, int32 Seconds, bool SetIsActive);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddToStats(int Amount, EGameStats GameStats, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddPart(ETeams Team, EPart Part);
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddResource(int Amount, EResources Resource, ETeams Team);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void RemoveResource(int Amount, EResources Resource, ETeams Team);

	//Called when player wants to check ready, and does so in the server
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerCheckReady(ETeams Team);
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
	void ClientCheckReady(ETeams Team);

	UFUNCTION(Server, reliable)
	void ServerMultiCastActivateTimer();
	
	UFUNCTION(NetMulticast, reliable, BlueprintCallable)
	void MultiCastActivateTimer(float T,  bool SetIsActive);
	
};
