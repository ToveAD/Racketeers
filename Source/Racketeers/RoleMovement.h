// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoleMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACKETEERS_API URoleMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoleMovement();

	UFUNCTION(BlueprintCallable, Category="Gunner")
	void RotateTurret(FVector LookAtTarget);

	UFUNCTION(BlueprintCallable, Category="Gunner")
	void RotateTurretTowardsMouse();

	UFUNCTION(BlueprintCallable, Category="Gunner")
	void Fire();

private:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;
		
};
