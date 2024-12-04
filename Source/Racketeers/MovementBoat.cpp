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

    // Get the BoatMesh component for movement and rotation updates
    BoatMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
    if (!BoatMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("BoatMesh not found!"));
        return;
    }

    // Initialize replicated variables to match current transform
    ReplicatedLocation = BoatMesh->GetComponentLocation();
    ReplicatedRotation = BoatMesh->GetComponentRotation();
}

void UMovementBoat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwner()->HasAuthority())
    {
        // Server-side movement handling
        if (bShouldMove)
        {
            RotateToFaceDirection(MovementInput);
            MoveForward(DeltaTime, bScurryIsActive);
            SimulateWaves(DeltaTime, MovingWaveHeight, MovingWaveFrequency);
        }
        else
        {
            SimulateWaves(DeltaTime, PassiveWaveHeight, PassiveWaveFrequency);
        }

        // Update transform replication to clients
        Server_UpdateTransform(DeltaTime);
    }
    else if (bShouldMove)
    {
        // Client-side interpolation for smoother movement
        Client_InterpolateTransform(DeltaTime);
    }
}

void UMovementBoat::Move(const FVector2D Value, bool bStarted)
{
    FindCameraAndSpringArm();
    bShouldMove = bStarted;

    if (bStarted)
    {
        MovementInput = Value;
        CurrentSpeed = MovementSpeed;
    }

    // If this is the client, send input to the server
    if (GetOwnerRole() < ROLE_Authority)
    {
        Server_Move(Value, bStarted);
    }
}

void UMovementBoat::Server_Move_Implementation(const FVector2D Value, bool bStarted)
{
    Move(Value, bStarted);
}

bool UMovementBoat::Server_Move_Validate(const FVector2D Value, bool bStarted)
{
    return true;
}

void UMovementBoat::Scurry(bool bIsScurrying)
{
    bScurryIsActive = bIsScurrying;
    if (GetOwnerRole() < ROLE_Authority)
    {
        // If this is the client, send scurry state to the server
        Server_Scurry(bIsScurrying);
    }
}

void UMovementBoat::Server_Scurry_Implementation(bool bIsScurrying)
{
    Scurry(bIsScurrying);
}

void UMovementBoat::RotateToFaceDirection(const FVector2D& InputDirection)
{
    if (InputDirection.IsNearlyZero())
    {
        return;
    }

    // Get the direction the boat should face in world space
    FVector TargetDirection = GetWorldSpaceDirection(InputDirection);

    if (TargetDirection.IsNearlyZero())
    {
        return;
    }

    // Get current and target rotations
    FRotator CurrentRotation = BoatMesh->GetComponentRotation();
    FRotator TargetRotation = TargetDirection.Rotation();
    TargetRotation.Pitch = 0.0f;
    TargetRotation.Roll = 0.0f;

    // Interpolate smoothly between current and target rotations
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
    BoatMesh->SetWorldRotation(NewRotation, true);
}

void UMovementBoat::MoveForward(float DeltaTime, bool bScurryActive)
{
    // Get the direction the boat should move in world space
    FVector DesiredDirection = GetWorldSpaceDirection(MovementInput);

    if (DesiredDirection.IsNearlyZero())
    {
        return;
    }

    // Adjust the speed multiplier based on scurry state
    static float CurrentScurryMultiplier = 1.0f;
    float TargetScurryMultiplier = bScurryActive ? ScurryAmount : 1.0f;
    CurrentScurryMultiplier = FMath::FInterpTo(CurrentScurryMultiplier, TargetScurryMultiplier, DeltaTime, 2.0f);

    // Calculate the new location for the boat
    FVector CurrentLocation = BoatMesh->GetComponentLocation();
    FVector NewLocation = CurrentLocation + (DesiredDirection * CurrentSpeed * CurrentScurryMultiplier * DeltaTime);

    // Move the boat to the new location, considering collisions
    FHitResult HitResult;
    BoatMesh->SetWorldLocation(NewLocation, true, &HitResult);

    // Update replicated location and rotation on the server
    if (GetOwnerRole() == ROLE_Authority)
    {
        ReplicatedLocation = NewLocation;
        ReplicatedRotation = BoatMesh->GetComponentRotation();
    }
}

FVector UMovementBoat::GetWorldSpaceDirection(const FVector2D& InputDirection) const
{
    if (!SpringArm)
    {
        UE_LOG(LogTemp, Error, TEXT("SpringArm is null! Using default forward direction."));
        return FVector::ForwardVector;
    }

    // Get the rotation of the spring arm to determine the direction the boat should move
    FRotator SpringArmRotation = SpringArm->GetComponentRotation();
    FRotator YawRotation(0.0f, SpringArmRotation.Yaw, 0.0f);

    // Calculate forward and right vectors based on yaw rotation
    FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // Convert input direction to world space direction
    FVector WorldSpaceDirection = (ForwardVector * InputDirection.Y + RightVector * InputDirection.X).GetClampedToMaxSize(1.0f);
    return WorldSpaceDirection;
}

void UMovementBoat::SimulateWaves(float DeltaTime, float WaveHeight, float WaveFrequency)
{
    // Update the boat's Z location to simulate wave movement
    FVector CurrentLocation = BoatMesh->GetComponentLocation();

    // Accumulate wave time for smooth wave simulation
    WaveTimeAccumulator += DeltaTime * WaveFrequency;
    float WaveOffset = FMath::Sin(WaveTimeAccumulator) * WaveHeight;

    CurrentLocation.Z += WaveOffset;
    BoatMesh->SetWorldLocation(CurrentLocation, true);
}

void UMovementBoat::FindCameraAndSpringArm()
{
    TeamCamera = nullptr;
    SpringArm = nullptr;

    // Determine which camera to use based on the boat's team tag
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

    // Find the camera and spring arm components based on the tag
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

    // Interpolate the boat's location and rotation to make movement appear smooth on the client side
    FVector CurrentLocation = BoatMesh->GetComponentLocation();
    FRotator CurrentRotation = BoatMesh->GetComponentRotation();

    FVector InterpolatedLocation = FMath::VInterpTo(CurrentLocation, ReplicatedLocation, DeltaTime, 5.0f);
    FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, ReplicatedRotation, DeltaTime, 5.0f);

    BoatMesh->SetWorldLocation(InterpolatedLocation, true);
    BoatMesh->SetWorldRotation(InterpolatedRotation, true);
}

void UMovementBoat::Server_UpdateTransform_Implementation(float DeltaTime)
{
    if (bShouldMove)
    {
        // Update the boat's location and rotation on the server
        FVector DesiredDirection = GetWorldSpaceDirection(MovementInput);
        FVector NewLocation = BoatMesh->GetComponentLocation() + (DesiredDirection * CurrentSpeed * DeltaTime);

        BoatMesh->SetWorldLocation(NewLocation, true);
        ReplicatedLocation = NewLocation;
        ReplicatedRotation = BoatMesh->GetComponentRotation();
    }
}

bool UMovementBoat::Server_UpdateTransform_Validate(float DeltaTime)
{
    return true;
}

void UMovementBoat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UMovementBoat, ReplicatedLocation);
    DOREPLIFETIME(UMovementBoat, ReplicatedRotation);
}
