#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatMovement.h" // Include BoatMovement to access movement functions
#include "DriverAccelerateProxy.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RACKETEERS_API UDriverAccelerateProxy : public UActorComponent
{
	GENERATED_BODY()

public:
	UDriverAccelerateProxy();

	UFUNCTION(BlueprintCallable, Category="Movement")
	void HandleAccelerationInput(float Value);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerHandleAcceleration(float Value);

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	AActor* Boat;

	UPROPERTY(Replicated)
	UBoatMovement* BoatMovementComponent;
};