// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "LobbySpawnPoint.generated.h"

UCLASS()
class RACKETEERS_API ALobbySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbySpawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* PlayerSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* NameTagSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	APlayerController* PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> PandaPlayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> RaccoonPlayerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	AActor* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamID = -1;
	

	UFUNCTION()
	void SpawnPlayer(APlayerController* PC, ETeams Team);

	UFUNCTION()
	void RemovePlayer();
	
	
};
