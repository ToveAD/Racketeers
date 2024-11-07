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

    // You can add any logic here that needs to be applied each frame.
}

void UBoatMovement::Accelerate(float Value)
{
    if (GetOwner()->HasAuthority()) // If on the server, apply directly
    {
        FVector ForwardVector = GetOwner()->GetActorForwardVector();
        ForwardVector.Z = 0.0f;  // Zero out the Z axis for forward movement only

        FVector ForwardForce = ForwardVector * Value * BoatSpeed;
        BoatMesh->AddForce(ForwardForce, NAME_None, true);  // Apply force to the boat
    }
    else // If on the client, request the server to apply the force
    {
        ServerAccelerate(Value);
    }
}

void UBoatMovement::Steer(float Value)
{
    if (GetOwner()->HasAuthority()) // If on the server, apply directly
    {
        FVector Torque = FVector(0.0f, 0.0f, Value * SteeringSpeed);  // Apply torque for steering on the Z-axis
        BoatMesh->AddTorqueInDegrees(Torque, NAME_None, true);  // Apply torque to the boat
    }
    else // If on the client, request the server to apply the torque
    {
        ServerSteer(Value);
    }
}

// Server implementation for Accelerate
void UBoatMovement::ServerAccelerate_Implementation(float Value)
{
    Accelerate(Value);
}

bool UBoatMovement::ServerAccelerate_Validate(float Value)
{
    return true;  // You can add specific validation here if needed
}

// Server implementation for Steer
void UBoatMovement::ServerSteer_Implementation(float Value)
{
    Steer(Value);
}

bool UBoatMovement::ServerSteer_Validate(float Value)
{
    return true;  // You can add specific validation here if needed
}

// Teleport the boat when moving it
void UBoatMovement::TeleportBoat(const FVector& NewLocation)
{
    if (BoatMesh)
    {
        BoatMesh->SetWorldLocation(NewLocation, true);  // Use 'true' for the Teleport flag to move without affecting physics
    }
}
