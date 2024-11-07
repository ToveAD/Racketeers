#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatMovement.generated.h"

// Forward declarations
class UPrimitiveComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RACKETEERS_API UBoatMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoatMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Method to accelerate the boat
	UFUNCTION(BlueprintCallable, Category="BoatMovement")
	void Accelerate(float Value);

	// Method to steer the boat
	UFUNCTION(BlueprintCallable, Category="BoatMovement")
	void Steer(float Value);

	// Server-side methods to handle movement in multiplayer
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAccelerate(float Value);
	void ServerAccelerate_Implementation(float Value);
	bool ServerAccelerate_Validate(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSteer(float Value);
	void ServerSteer_Implementation(float Value);
	bool ServerSteer_Validate(float Value);

	// Teleport the boat to a new location (with teleport flag)
	UFUNCTION(BlueprintCallable)
	void TeleportBoat(const FVector& NewLocation);

private:
	// The mesh of the boat (used for physics)
	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* BoatMesh;

	// Boat's speed and steering multipliers (can be adjusted in the editor)
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoatSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SteeringSpeed = 50.0f;
};
