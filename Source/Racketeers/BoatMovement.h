#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputMappingContext.h" // Include for input mapping contexts
#include "BoatMovement.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RACKETEERS_API UBoatMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	UBoatMovement();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Steer(float Value);

	void TeleportBoat(const FVector& NewLocation);

	UFUNCTION(BlueprintCallable, Category="Input")
	void SwitchInputMapping(bool IsAttaching);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAccelerate(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSteer(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAccelerate(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSteer(float Value);

	UFUNCTION(Client, Reliable)
	void ClientSwitchInputMapping(bool IsAttaching);

	UFUNCTION(Server, Reliable)
	void ServerSwitchInputMapping(bool IsAttaching);

private:
	UPROPERTY(EditAnywhere, Category="Movement")
	float BoatSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float SteeringSpeed = 500.0f;

	UPROPERTY()
	UPrimitiveComponent* BoatMesh;

	UPROPERTY()
	UInputMappingContext* IMC_Boat;    // Declare IMC_Boat

	UPROPERTY()
	UInputMappingContext* IMC_Default; // Declare IMC_Default
};
