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
	void SwitchInputMapping(bool IsAttaching,  UInputMappingContext* InputToAdd, UInputMappingContext* InputToRemove, APlayerController* PlayerController);

private:
	UPROPERTY(EditAnywhere, Category="Movement")
	float BoatSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float SteeringSpeed = 500.0f;

	// Boat maximum speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxBoatSpeed = 1200.0f; // Maximum speed of the boat

	// Drag force to slow down the boat when no input is applied
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float DragForce = 5.0f; // Higher values result in faster deceleration

	// Rapid deceleration force multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RapidDecelerationForce = 20.0f; // Higher value = faster deceleration

	UPROPERTY()
	UPrimitiveComponent* BoatMesh;

	UPROPERTY()
	UInputMappingContext* IMC_Boat;    // Declare IMC_Boat

	UPROPERTY()
	UInputMappingContext* IMC_Default; // Declare IMC_Default

	// Flags for input control
	bool bIsAccelerating = false; // True when acceleration input is active
	float CurrentAccelerationValue = 0.0f; // Holds the current acceleration input value
};
