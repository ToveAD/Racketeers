// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerInfo.h"

ATimerInfo::ATimerInfo()
{
	bIsActive = false;
	Time = 0;
}


void ATimerInfo::Tick(float DeltaSeconds)
{
	if(bIsActive)
	{
		Super::Tick(DeltaSeconds);
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

void ATimerInfo::ActivateTime()
{
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
