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
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	int TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	bool bIsOccupied = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> PandaPlayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> RaccoonPlayerClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPlayer(ETeams Team);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
