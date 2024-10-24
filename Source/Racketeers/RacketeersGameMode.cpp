// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacketeersGameMode.h"
#include "RacketeersCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARacketeersGameMode::ARacketeersGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
