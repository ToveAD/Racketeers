// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CannonHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACKETEERS_API UCannonHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCannonHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void ChooseCannon(int32 CannonIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UNiagaraComponent* EffectIndex;

	//UPROPERTY(EditAnywhere, Category="CannonFunctions")
	//void FireCannon(bool ShootLeft, int32 CannonIndex);

	//UPROPERTY(EditAnywhere, Category="CannonComponents")
	//USceneComponent* ProjectileSpawnPoint;

	//UPROPERTY(EditAnywhere, Category="CannonComponents")
	//USceneComponent* CannonIndex;
		
};
