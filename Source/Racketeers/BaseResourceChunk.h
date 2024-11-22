// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModeStructs.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interactable.h"
#include "BaseResourceChunk.generated.h"

UCLASS()
class RACKETEERS_API ABaseResourceChunk : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseResourceChunk();
	
	ABaseResourceChunk(FVector velocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//MeshComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* ResourceMesh;
	//ProjectileComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UProjectileMovementComponent* ProjectileMovement;

	//Velocity var, will be used to set projectileComponent velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileInitialVariables", meta = (ExposeOnSpawn))
	FVector velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ProjectileInitialVariables")
	float throwForce = 100000.0f;
	//ResourceChunk Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ResourceInfo")
	EResources type = EResources::NONE;
	//ResourceChunk Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ResourceInfo")
	int value;
	//ResourceChunk LastOwner, is used to remember the last actor who held the ResourceChunk 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ResourceInfo")
	AActor* LastOwner = nullptr;
	//Bool to prevent multiple actors to hold the ResourceChunk
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ResourceInfo")
	bool isBeingHeld = false;	
	
	// Floor Check function (virtual and blueprint callable)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Resource")
	bool GroundCheck();
	virtual bool GroundCheck_Implementation();
	
	
};
