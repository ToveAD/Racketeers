// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PS_Base.h"
#include "PS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class RACKETEERS_API APS_Lobby : public APS_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	ETeams Team;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
