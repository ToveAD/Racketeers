// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GS_Base.h"
#include "GS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API AGS_Lobby : public AGS_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TArray<APlayerController*> RaccoonPlayers;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TArray<APlayerController*> PandaPlayers;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
