#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h" // Required for Enhanced Input
#include "MovementBoat.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RACKETEERS_API UMovementBoat : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovementBoat();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function to handle input
	UFUNCTION(BlueprintCallable, Category="Movement")
	void Move(FVector2D Value, bool bStarted);

private:
	void RotateToFaceDirection(const FVector2D& InputDirection);
	void MoveForward(float DeltaTime);

	// Movement properties
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 5.0f; // Speed of rotation smoothing

	FVector2D MovementInput = FVector2D::ZeroVector;
	bool bShouldMove = false;

	// Replicated properties for movement
	FVector ReplicatedLocation;
	FRotator ReplicatedRotation;

	// Interpolation speed for clients
	UPROPERTY(EditAnywhere, Category = "Replication")
	float InterpolationSpeed = 5.0f;
};
