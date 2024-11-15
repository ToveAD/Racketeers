// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "TimerInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RACKETEERS_API ATimerInfo : public AInfo
{
	GENERATED_BODY()


	ATimerInfo();



	void DecreaseTime(float DeltaSeconds);
	
	bool bIsActive;
	
	public:
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Blueprintable)
	void SetTimeSeconds(float seconds, bool SetIsActive);
	UFUNCTION(BlueprintCallable, Blueprintable)
	void SetTime_Analog(int32 Minutes, int32 Seconds, bool SetIsActive);

	
	UFUNCTION(BlueprintCallable, Blueprintable)
	int32 GetMinutes();
	UFUNCTION(BlueprintCallable, Blueprintable)
	int32 GetSeconds();

	UFUNCTION()
	void On_RepStartTimer();
	
	UFUNCTION(NetMulticast, reliable)
	void MultiCastActivateTimer(float T);
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	
	
	
	UFUNCTION(BlueprintCallable, Blueprintable)
	void ActivateTime();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Time;


	UPROPERTY(ReplicatedUsing=On_RepStartTimer, BlueprintReadWrite)
	float StartTimer;


};

