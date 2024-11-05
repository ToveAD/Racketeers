#include "BoatMovement.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

UBoatMovement::UBoatMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoatMovement::BeginPlay()
{
	Super::BeginPlay();

	// Get the StaticMesh or any other PrimitiveComponent that simulates physics
	BoatMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));

	if (BoatMesh)
	{
		BoatMesh->SetSimulatePhysics(true);  // Ensure that physics simulation is enabled
	}
}

void UBoatMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBoatMovement::Accelerate(float Value)
{
	// Get the forward vector of the boat and ignore the Z component (to restrict movement to X and Y axes)
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	ForwardVector.Z = 0.0f;  // Zero out the Z axis

	// Apply forward force only on the X and Y axes
	FVector ForwardForce = ForwardVector * Value * BoatSpeed;

	// Add force to the boat mesh
	BoatMesh->AddForce(ForwardForce, NAME_None, true);
}

void UBoatMovement::Steer(float Value)
{
	// Apply torque for steering
	FVector Torque = FVector(0.0f, 0.0f, Value);
	BoatMesh->AddTorqueInDegrees(Torque, NAME_None, true);
}



