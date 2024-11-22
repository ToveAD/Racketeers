#include "MovementBoat.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"

UMovementBoat::UMovementBoat()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UMovementBoat::BeginPlay()
{
    Super::BeginPlay();

    // Initialize replicated location and rotation
    ReplicatedLocation = GetOwner()->GetActorLocation();
    ReplicatedRotation = GetOwner()->GetActorRotation();
}

void UMovementBoat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if(bShouldMove)
    {
        RotateToFaceDirection(MovementInput);
        MoveForward(DeltaTime);
    }
}

// Function to handle movement input
void UMovementBoat::Move(FVector2D Value, bool bStarted)
{
    bShouldMove = bStarted;
    MovementInput = Value;
}

// Rotate the boat to face the input direction
void UMovementBoat::RotateToFaceDirection(const FVector2D& InputDirection)
{
    if (!InputDirection.IsNearlyZero())
    {
        // Calculate the target rotation based on the input direction
        FRotator CurrentRotation = GetOwner()->GetActorRotation();
        FVector TargetDirection(InputDirection.X, InputDirection.Y, 0.0f);
        TargetDirection.Normalize();

        FRotator TargetRotation = TargetDirection.Rotation();
        TargetRotation.Pitch = 0.0f; // Ensure no pitch rotation
        TargetRotation.Roll = 0.0f;

        // Smoothly rotate to the target rotation
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
        GetOwner()->SetActorRotation(NewRotation);
    }
}

// Move the boat forward
void UMovementBoat::MoveForward(float DeltaTime)
{
    FVector ForwardDirection = GetOwner()->GetActorForwardVector();
    FVector NewLocation = GetOwner()->GetActorLocation() + (ForwardDirection * MovementSpeed * DeltaTime);
    GetOwner()->SetActorLocation(NewLocation, true);
}
