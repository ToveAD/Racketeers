#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatMovement.h" // Include BoatMovement to access movement functions
#include "GunnerProxy.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RACKETEERS_API UGunnerProxy : public UActorComponent
{
	GENERATED_BODY()

public:
	UGunnerProxy();

	void HandleGunnerProxy(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHandleGunner(float Value);

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	AActor* Boat;

	UPROPERTY(Replicated)
	UBoatMovement* BoatMovementComponent;
};
