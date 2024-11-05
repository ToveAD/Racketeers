// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

	/*
	 *	Fases class
	 *
	 *
	 *	Phase 1
	 *		- Next Face condition is a time limit.
	 *	Phase 2
	 *		- Next Face Condition is a time limit.
	 *	Phase 3
	 *		- Determinig end of face is a timer
	 *		- lose condition is when the teams ship is destroyed or the team has less health than the other team
	 *		- win condition is when the other teams ship is destroyed or the team has more health than the other
	 *
	 */

class RACKETEERS_API Phase
{
public:
	Phase();
	~Phase();

	UPROPERTY(BlueprintReadWrite)
	float TimeLimit;

	//Team_A Struct
	//Team_B Struct
	
	void LoseCondition();
	void WinCondition();
	
};
