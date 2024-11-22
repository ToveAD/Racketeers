// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Components/ArrowComponent.h"
#include "Components/Image.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* SteamProfileImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	FString SteamName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	int TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	bool bIsOccupied = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> PandaPlayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> RaccoonPlayerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	AActor* Player = nullptr;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnPlayer(ETeams Team);

	UFUNCTION()
	void RemovePlayer();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
