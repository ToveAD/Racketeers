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

	// Input Mapping Contexts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Boat;  // Boat-specific input context

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Default; // Default input context

	// Boat Movement Functions
	UFUNCTION(BlueprintCallable, Category = "BoatMovement")
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, Category = "BoatMovement")
	void Steer(float Value);

	// Networked Movement Functions (Server-Side)
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAccelerate(float Value);
	void ServerAccelerate_Implementation(float Value);
	bool ServerAccelerate_Validate(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSteer(float Value);
	void ServerSteer_Implementation(float Value);
	bool ServerSteer_Validate(float Value);

	// Boat Teleport Function
	void TeleportBoat(const FVector& NewLocation);

	// Input Mapping Management
	UFUNCTION(BlueprintCallable, Category = "BoatMovement")
	void SwitchInputMapping(bool IsAttaching);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientSwitchInputMapping(bool IsAttaching);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSwitchInputMapping(bool IsAttaching);

	// Apply input mapping directly on the client
	void ApplyInputMapping(bool IsAttaching);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Boat mesh (or any other component you use for physics)
	UPROPERTY()
	UPrimitiveComponent* BoatMesh;

	// Boat Movement Properties
	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float BoatSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float SteeringSpeed = 100.0f;
};
