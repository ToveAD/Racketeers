#include "MovementBoat.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameStateBase.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "InputActionValue.h"
#include "PS_Base.h"

UMovementBoat::UMovementBoat()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);

    bShouldMove = false;
    bScurryIsActive = false;
    CurrentSpeed = 0.0f;
}

void UMovementBoat::BeginPlay()
{
    Super::BeginPlay();
    //FindCameraAndSpringArm();

    // Get BoatMesh for all transform updates
    BoatMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
    if (!BoatMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("BoatMesh not found!"));
        return;
    }

    // Initialize replication variables
    ReplicatedLocation = BoatMesh->GetComponentLocation();
    ReplicatedRotation = BoatMesh->GetComponentRotation();
}

void UMovementBoat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwner()->HasAuthority() && BoatMesh->IsCollisionEnabled()) // Server side
    {
        if (bShouldMove)
        {
            RotateToFaceDirection(MovementInput);
            MoveForward(DeltaTime, bScurryIsActive);
            SimulateWaves(DeltaTime, MovingWaveHeight, MovingWaveFrequency);
        }else if(!bShouldMove)
        {
            SimulateWaves(DeltaTime, PassiveWaveHeight, PassiveWaveFrequency);
        }

        Server_UpdateTransform(DeltaTime); // Send transform updates to clients
    }
    else if(bShouldMove && BoatMesh->IsCollisionEnabled())// Client side
    {
        Client_InterpolateTransform(DeltaTime); // Smooth client-side interpolation
    }
}

void UMovementBoat::Move(FVector2D Value, bool bStarted)
{
    FindCameraAndSpringArm();
    bShouldMove = bStarted;

    if (bStarted)
    {
        MovementInput = Value;
        CurrentSpeed = MovementSpeed;
    }

    if (GetOwnerRole() < ROLE_Authority) // Client sends input to the server
    {
        Server_Move(Value, bStarted);
    }
}

void UMovementBoat::Server_Move_Implementation(FVector2D Value, bool bStarted)
{
    Move(Value, bStarted);
}

bool UMovementBoat::Server_Move_Validate(FVector2D Value, bool bStarted)
{
    return true;
}

void UMovementBoat::Scurry(bool bIsScurrying)
{
    bScurryIsActive = bIsScurrying;
}

void UMovementBoat::RotateToFaceDirection(const FVector2D& InputDirection)
{
    if (!InputDirection.IsNearlyZero())
    {
        FVector TargetDirection = GetWorldSpaceDirection(InputDirection);

        if (TargetDirection.IsNearlyZero())
        {
            return; // No valid direction to face
        }

        // Get the current and target rotations
        FRotator CurrentRotation = BoatMesh->GetComponentRotation();
        FRotator TargetRotation = TargetDirection.Rotation();
        TargetRotation.Pitch = 0.0f; // Ensure no pitch rotation
        TargetRotation.Roll = 0.0f;

        // Smoothly interpolate to the target rotation
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
        BoatMesh->SetWorldRotation(NewRotation, true);
    }
}

void UMovementBoat::MoveForward(float DeltaTime, bool bScurryActive)
{
    FVector DesiredDirection = GetWorldSpaceDirection(MovementInput);

    if (!DesiredDirection.IsNearlyZero())
    {
        // Gradually adjust the ScurryMultiplier
        static float CurrentScurryMultiplier = 1.0f; // Start at normal speed
        float TargetScurryMultiplier = bScurryActive ? ScurryAmount : 1.0f;

        // Smoothly interpolate to the target multiplier
        CurrentScurryMultiplier = FMath::FInterpTo(CurrentScurryMultiplier, TargetScurryMultiplier, DeltaTime, 2.0f);

        // Update the wave time accumulator
        //WaveTimeAccumulator += DeltaTime * MovingWaveFrequency;

        // Calculate the wave offset
       // float WaveOffset = FMath::Sin(WaveTimeAccumulator) * MovingWaveHeight;

        // Calculate the new location
        FVector CurrentLocation = BoatMesh->GetComponentLocation();
        FVector NewLocation = CurrentLocation + (DesiredDirection * CurrentSpeed * CurrentScurryMultiplier * DeltaTime);
        //NewLocation.Z += WaveOffset; // Apply the wave effect to the Z-axis

        // Move the boat with collision
        FHitResult HitResult;
        BoatMesh->SetWorldLocation(NewLocation, true, &HitResult);

        // Update replicated transform for clients
        if (GetOwnerRole() == ROLE_Authority)
        {
            ReplicatedLocation = NewLocation;
            ReplicatedRotation = BoatMesh->GetComponentRotation();
        }

        // Debugging: Log wave offset
        //UE_LOG(LogTemp, Log, TEXT("Wave Offset: %f"), WaveOffset);
    }
}

FVector UMovementBoat::GetWorldSpaceDirection(const FVector2D& InputDirection) const
{
    if (!SpringArm)
    {
        UE_LOG(LogTemp, Error, TEXT("SpringArm is null! Using default forward direction."));
        return FVector::ZeroVector;
    }

    // Get the spring arm's yaw rotation
    FRotator SpringArmRotation = SpringArm->GetComponentRotation();
    FRotator YawRotation(0.0f, SpringArmRotation.Yaw, 0.0f);

    // Calculate forward and right vectors based on the spring arm's yaw
    FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // Transform the input direction into a world space direction
    FVector WorldSpaceDirection = (ForwardVector * InputDirection.X + RightVector * InputDirection.Y).GetClampedToMaxSize(1.0f);
    return WorldSpaceDirection;
}

void UMovementBoat::SimulateWaves(float DeltaTime, float WaveHeight, float WaveFrequency)
{
    FVector CurrentLocation = BoatMesh->GetComponentLocation();

    // Update the wave time accumulator
    WaveTimeAccumulator += DeltaTime * WaveFrequency;

    // Calculate the wave offset
    float WaveOffset = FMath::Sin(WaveTimeAccumulator) * WaveHeight;

    // Apply the wave effect to the Z-axis
    CurrentLocation.Z += WaveOffset;

    BoatMesh->SetWorldLocation(CurrentLocation, true);
}


void UMovementBoat::FindCameraAndSpringArm()
{
    TeamCamera = nullptr;
    SpringArm = nullptr;

    FName CameraTag;
    if (GetOwner()->ActorHasTag("BoatPanda"))
    {
        CameraTag = FName("CameraPanda");
    }
    else if (GetOwner()->ActorHasTag("BoatRaccoon"))
    {
        CameraTag = FName("CameraRaccoon");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Boat does not have a valid team tag!"));
        return;
    }

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(CameraTag))
        {
            TeamCamera = *It;
            SpringArm = TeamCamera->FindComponentByClass<USpringArmComponent>();
            return;
        }
    }
}

void UMovementBoat::Client_InterpolateTransform(float DeltaTime)
{
    if (!BoatMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("BoatMesh is null in Client_InterpolateTransform!"));
        return;
    }

    // Accumulate time since last interpolation
    static float TimeSinceLastUpdate = 0.0f;
    TimeSinceLastUpdate += DeltaTime;

    // Define a lerp alpha to control smoothing
    const float LerpSpeed = 5.0f; // Adjust for smoother transitions
    float Alpha = FMath::Clamp(TimeSinceLastUpdate * LerpSpeed, 0.0f, 1.0f);

    // Current position and rotation
    FVector CurrentLocation = BoatMesh->GetComponentLocation();
    FRotator CurrentRotation = BoatMesh->GetComponentRotation();

    // Interpolate location and rotation
    FVector InterpolatedLocation = FMath::Lerp(CurrentLocation, ReplicatedLocation, Alpha);
    FRotator InterpolatedRotation = FMath::Lerp(CurrentRotation, ReplicatedRotation, Alpha);

    // Update the boat mesh
    BoatMesh->SetWorldLocation(InterpolatedLocation, true);
    BoatMesh->SetWorldRotation(InterpolatedRotation, true);

    UpdateComponentToWorld();

    // Reset the interpolation time when fully updated
    if (Alpha >= 1.0f)
    {
        TimeSinceLastUpdate = 0.0f;
    }
}

void UMovementBoat::Server_UpdateTransform_Implementation(float DeltaTime)
{
    if (bShouldMove)
    {
        FVector DesiredDirection = GetWorldSpaceDirection(MovementInput);
        FVector NewLocation = BoatMesh->GetComponentLocation() + (DesiredDirection * CurrentSpeed * DeltaTime);

        BoatMesh->SetWorldLocation(NewLocation, true);
        ReplicatedLocation = NewLocation;
        ReplicatedRotation = BoatMesh->GetComponentRotation();
        UpdateComponentToWorld();
    }
}

bool UMovementBoat::Server_UpdateTransform_Validate(float DeltaTime)
{
    return true;
}

void UMovementBoat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UMovementBoat, bShouldMove);
    DOREPLIFETIME(UMovementBoat, MovementInput);
    DOREPLIFETIME(UMovementBoat, CurrentSpeed);
    DOREPLIFETIME(UMovementBoat, bScurryIsActive);
    DOREPLIFETIME(UMovementBoat, ReplicatedLocation);
    DOREPLIFETIME(UMovementBoat, ReplicatedRotation);
}
