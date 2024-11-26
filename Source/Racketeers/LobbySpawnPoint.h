// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeStructs.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "LobbySpawnPoint.generated.h"


UCLASS()
class RACKETEERS_API ALobbySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbySpawnPoint();

	// ----------------------------Variables--------------------------------------------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* PlayerSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* LobbyInfoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	APlayerController* PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> PandaPlayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team")
	TSubclassOf<AActor> RaccoonPlayerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	AActor* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* SpawnVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int TeamID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_bShowPlayerInfo")
	bool bShowPlayerInfo = false;

	// ----------------------------Functions--------------------------------------------
	
	UFUNCTION()
	void SpawnPlayer(APlayerController* PC, ETeams Team);

	UFUNCTION()
	void RemovePlayer();

	//UFUNCTION()
	//void UpdateWidgetInfo(FLobbyInfo NewLobbyInfo);

	UFUNCTION()
	void OnRep_bShowPlayerInfo();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
