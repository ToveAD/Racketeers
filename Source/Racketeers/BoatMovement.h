#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UObject/SoftObjectPtr.h"
#include "BoatMovement.generated.h"

// Forward declarations
class APlayerController;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RACKETEERS_API UBoatMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoatMovement();

	// Direct reference to the InputMappingContext assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Boat;

	// Default InputMappingContext asset reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Default;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Movement functions
	UFUNCTION(BlueprintCallable, Category="BoatMovement")
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, Category="BoatMovement")
	void Steer(float Value);

	// Server function declarations
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAccelerate(float Value);
	void ServerAccelerate_Implementation(float Value);
	bool ServerAccelerate_Validate(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSteer(float Value);
	void ServerSteer_Implementation(float Value);
	bool ServerSteer_Validate(float Value);


	// Teleport the boat
	void TeleportBoat(const FVector& NewLocation);

	// Switch to boat-specific input mapping context
	UFUNCTION(BlueprintCallable, Category="BoatMovement")
	void SwitchToBoatInputMapping();

private:
	// Boat mesh (or any other component you use for physics)
	UPROPERTY()
	UPrimitiveComponent* BoatMesh;

	// Boat movement properties (for example, speed and steering)
	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float BoatSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float SteeringSpeed = 100.0f;
};
