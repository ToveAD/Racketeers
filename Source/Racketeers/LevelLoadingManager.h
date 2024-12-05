// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/Actor.h"
#include "LevelLoadingManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnloadLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingLevelCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnloadingLevelCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingLevelFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnloadingLevelFailed);

class UPhase;

UCLASS()
class RACKETEERS_API ALevelLoadingManager : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadLevel OnLoadLevel;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnUnloadLevel OnUnloadLevel;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadingLevelCompleted OnLoadingLevelCompleted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnUnloadingLevelCompleted OnUnloadingLevelCompleted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLoadingLevelFailed OnLoadingLevelFailed;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnUnloadingLevelFailed OnUnloadingLevelFailed;
	
	// Sets default values for this actor's properties
	ALevelLoadingManager();

	UFUNCTION(NetMulticast, Reliable, WithValidation , BlueprintCallable)
	void MulticastLoadLevel(const UPhase* LevelPath);

	void LoadLevel();
	void UnloadLevel();
	
	void LoadLevelParent();

	void UnloadLevelParent();

	void LoadSubLevel();

	void UnloadSubLevel();
	
	bool bLevelLoadSuccessfull;

	bool bLevelIsLoading;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void LevelUnloaded();
	UFUNCTION()
	void LevelLoaded();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentLevelPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentSubLevelPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NextLevelPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NextSubLevelPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhase* CurrentPhase;

private:
	UPROPERTY()
	ULevelStreamingDynamic* CurrentLoadedLevel;


};
