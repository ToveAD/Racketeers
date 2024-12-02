#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatSettings.h"
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

	// Struct variable exposed to Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	FBoatSettings BoatSettings;

	// BlueprintCallable functions for Acceleration and Steering
	UFUNCTION(BlueprintCallable, Category="Movement")
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, Category="Movement")
	void Steer(float Value);

	// Server function to update the position and rotation for replication
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateTransform(float DeltaTime);

	// Client-side function to interpolate towards the target position and rotation
	void Client_InterpolateTransform(float DeltaTime);

	// Server functions for Accelerate and Steer
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Accelerate(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Steer(float Value);

private:
	// Helper functions for applying movement and steering
	void ApplyMovement(float Value, bool GoBackwards);
	void ApplySteering(float Value);

	// Maximum speed for boat movement
	UPROPERTY(EditAnywhere, Category="Movement")
	float BoatSpeed = 1000.0f;
	
	float CurrentSpeed = 0.0f; // Boat's current speed, starts at 0

	// Steering speed for turning the boat
	UPROPERTY(EditAnywhere, Category="Movement")
	float SteeringSpeed = 500.0f;

	// Interpolation speed for the client
	UPROPERTY(EditAnywhere, Category="Interpolation")
	float InterpolationSpeed = 5.0f;

	// Threshold for distance or angle correction
	UPROPERTY(EditAnywhere, Category="Interpolation")
	float CorrectionThreshold = 50.0f;

	// Minimum change in position to trigger replication
	UPROPERTY(EditAnywhere, Category="Interpolation")
	float UpdateThreshold = 50.0f;

	// Replicated position and rotation for clients to interpolate towards
	UPROPERTY(Replicated)
	FVector ReplicatedLocation;

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;
	
	// Reference to the boat’s mesh component
	UPROPERTY()
	UPrimitiveComponent* BoatMesh;
};
