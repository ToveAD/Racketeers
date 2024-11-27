#include "BoatMovement.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

UBoatMovement::UBoatMovement()
{
    PrimaryComponentTick.bCanEverTick = true; // Enable ticking for this component
    SetIsReplicatedByDefault(true); // Enable replication for this component
}

void UBoatMovement::BeginPlay()
{
    Super::BeginPlay();

    BoatMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));

    if (BoatMesh)
    {
        BoatMesh->SetSimulatePhysics(true);
        BoatMesh->SetIsReplicated(true);
        BoatMesh->GetOwner()->SetReplicateMovement(false); // Disable default movement replication; we’ll handle it manually
    }

    // Initialize target location and rotation
    ReplicatedLocation = GetOwner()->GetActorLocation();
    ReplicatedRotation = GetOwner()->GetActorRotation();
}

void UBoatMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwner()->HasAuthority()) // Server side
    {
        Server_UpdateTransform(DeltaTime); // Update the authoritative transform on the server
    }
    else // Client side
    {
        Client_InterpolateTransform(DeltaTime); // Interpolate on the client
    }
}

// Accelerate Function
void UBoatMovement::Accelerate(float Value)
{
    if (Value == 0.0f)
    {
        return; // No input to process
    }

    if (GetOwner()->HasAuthority()) // Server processes movement
    {
        ApplyMovement(Value, false);
    }
    else // Client sends input to server
    {
        Server_Accelerate(Value);
        ApplyMovement(Value, false); // Client-side prediction
    }
}

// Steer Function
void UBoatMovement::Steer(float Value)
{
    if (Value == 0.0f)
    {
        return; // No input to process
    }

    if (GetOwner()->HasAuthority()) // Server processes steering
    {
        ApplySteering(Value);
    }
    else // Client sends input to server
    {
        Server_Steer(Value);
        ApplySteering(Value); // Client-side prediction
    }
}

// ApplyMovement Helper Function
void UBoatMovement::ApplyMovement(float Value, bool GoBackwards)
{
    if (BoatMesh)
    {
        FVector ForwardVector = GetOwner()->GetActorForwardVector();
        ForwardVector.Z = 0.0f; // Prevent vertical movement
        ForwardVector.Normalize();
        
        // Ensure the boat moves at a fixed speed when input is pressed
        if (Value == 1) // Value represents forward input
        {
            // Calculate the desired velocity
            FVector DesiredVelocity = ForwardVector * BoatSpeed;

            // Set the physics linear velocity directly to achieve constant speed
            BoatMesh->SetPhysicsLinearVelocity(DesiredVelocity, true);
        }
        else if (Value == -1) // Backward Movement Logic
        {
            // Calculate the desired velocity
            FVector DesiredVelocity = ForwardVector * -BoatSpeed;

            // Set the physics linear velocity directly to achieve constant speed
            BoatMesh->SetPhysicsLinearVelocity(DesiredVelocity, true);
        }

        // Update replicated values on the server
        if (GetOwner()->HasAuthority())
        {
            ReplicatedLocation = GetOwner()->GetActorLocation();
            ReplicatedRotation = GetOwner()->GetActorRotation();
        }
    }
}

// ApplySteering Helper Function
void UBoatMovement::ApplySteering(float Value)
{
    if (BoatMesh)
    {
        FVector Torque = FVector(0.0f, 0.0f, Value * SteeringSpeed);
        BoatMesh->AddTorqueInDegrees(Torque, NAME_None, true);

        // Update replicated rotation on the server
        if (GetOwner()->HasAuthority())
        {
            ReplicatedRotation = GetOwner()->GetActorRotation();
        }
    }
}

// Server RPCs for Accelerate
void UBoatMovement::Server_Accelerate_Implementation(float Value)
{
    ApplyMovement(Value, false);
}

bool UBoatMovement::Server_Accelerate_Validate(float Value)
{
    return FMath::Abs(Value) <= 1.0f;
}

// Server RPCs for Steer
void UBoatMovement::Server_Steer_Implementation(float Value)
{
    ApplySteering(Value);
}

bool UBoatMovement::Server_Steer_Validate(float Value)
{
    return FMath::Abs(Value) <= 1.0f;
}

// Implementation for Server_UpdateTransform
void UBoatMovement::Server_UpdateTransform_Implementation(float DeltaTime)
{
    if (BoatMesh)
    {
        // Update position and rotation only if significant movement occurs
        if (FVector::Dist(ReplicatedLocation, GetOwner()->GetActorLocation()) > UpdateThreshold ||
            FMath::Abs(ReplicatedRotation.Yaw - GetOwner()->GetActorRotation().Yaw) > UpdateThreshold)
        {
            ReplicatedLocation = GetOwner()->GetActorLocation();
            ReplicatedRotation = GetOwner()->GetActorRotation();
        }
    }
}

bool UBoatMovement::Server_UpdateTransform_Validate(float DeltaTime)
{
    // Validation logic: Ensure DeltaTime is within a reasonable range
    return DeltaTime >= 0.0f && DeltaTime <= 1.0f;
}

void UBoatMovement::Client_InterpolateTransform(float DeltaTime)
{
    FVector CurrentLocation = GetOwner()->GetActorLocation();
    FRotator CurrentRotation = GetOwner()->GetActorRotation();

    // Interpolate location and rotation towards the target
    FVector InterpolatedLocation = FMath::VInterpTo(CurrentLocation, ReplicatedLocation, DeltaTime, InterpolationSpeed);
    FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, ReplicatedRotation, DeltaTime, InterpolationSpeed);

    // Set the actor’s location and rotation
    GetOwner()->SetActorLocation(InterpolatedLocation);
    GetOwner()->SetActorRotation(InterpolatedRotation);
}

void UBoatMovement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Declaration of variables to be replicated
    DOREPLIFETIME(UBoatMovement, ReplicatedLocation);
    DOREPLIFETIME(UBoatMovement, ReplicatedRotation);
}
