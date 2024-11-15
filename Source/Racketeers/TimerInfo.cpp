// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerInfo.h"

#include "Net/UnrealNetwork.h"

ATimerInfo::ATimerInfo()
{
	bIsActive = false;
	Time = 0;
	UE_LOG(LogTemp, Warning, TEXT("TimerInfo::ATimerInfo()"));
}


void ATimerInfo::Tick(float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("TimerInfo::Tick()"));
	if(GEngine)
	{
		FString TeamName = "TimerInfo::Tick()";
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TeamName);
	}
	Super::Tick(DeltaSeconds);
	if(bIsActive)
	{

		DecreaseTime(DeltaSeconds);
	}
}

void ATimerInfo::DecreaseTime(float DeltaSeconds)
{
	Time -= DeltaSeconds;
	if(Time <= 0)
	{
		bIsActive = false;
	}
}

void ATimerInfo::SetTimeSeconds(float seconds, bool SetIsActive)
{
	if(this == nullptr)
	{
		Time = seconds;
		bIsActive = SetIsActive;
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "TimerInfo::SetTimeSeconds");
	}
}

void ATimerInfo::SetTime_Analog(int32 Minutes, int32 Seconds, bool SetIsActive)
{
	Time = Minutes * 60;
	Time += Seconds;
	bIsActive = SetIsActive;
}

void ATimerInfo::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimerInfo, StartTimer);
}

void ATimerInfo::ActivateTime()
{
	bIsActive = true;
}

void ATimerInfo::On_RepStartTimer()
{
	
}




void ATimerInfo::MultiCastActivateTimer_Implementation(float T)
{
	Time = T;
	bIsActive = true;
}

int32 ATimerInfo::GetMinutes()
{
	return (int32)(Time/60);
}

int32 ATimerInfo::GetSeconds()
{
	
	return (int32)Time % 60 ;
}
