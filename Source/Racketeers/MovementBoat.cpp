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
}

void UMovementBoat::BeginPlay()
{
    Super::BeginPlay();
    //FindCameraAndSpringArm();
}

void UMovementBoat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Run movement logic if Player is triggering input
    if(bShouldMove)
    {
        RotateToFaceDirection(MovementInput);
        MoveForward(DeltaTime, bScurryIsActive);
    }else
    {
        // Gradually decrease speed to 0 when input stops
        if (CurrentSpeed > 0.0f)
        {
            CurrentSpeed = FMath::Max(CurrentSpeed - DecelerationRate * DeltaTime, 0.0f);

            // Continue moving in the current forward direction
            FVector ForwardDirection = GetOwner()->GetActorForwardVector();
            FVector NewLocation = GetOwner()->GetActorLocation() + (ForwardDirection * CurrentSpeed * DeltaTime);
            GetOwner()->SetActorLocation(NewLocation, true);
        }
    }
}

// Function to handle movement input
void UMovementBoat::Move(FVector2D Value, bool bStarted)
{
    FindCameraAndSpringArm();
    bShouldMove = bStarted;

    if(bStarted)
    {
        MovementInput = Value;
        CurrentSpeed = MovementSpeed;
    } 
}

void UMovementBoat::Scurry(bool bIsScurrying)
{
    bScurryIsActive = bIsScurrying;
}

// Rotate the boat to face the input direction
void UMovementBoat::RotateToFaceDirection(const FVector2D& InputDirection)
{
    if (!InputDirection.IsNearlyZero())
    {
        FVector TargetDirection = GetWorldSpaceDirection(InputDirection);

        if(TargetDirection.IsNearlyZero())
        {
            return; // No valid direction to face
        }

        // Get the current and target rotations
        FRotator CurrentRotation = GetOwner()->GetActorRotation();
        FRotator TargetRotation = TargetDirection.Rotation();
        TargetRotation.Pitch = 0.0f; // Ensure no pitch rotation
        TargetRotation.Roll = 0.0f;

        // Smoothly interpolate to the target rotation
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
        GetOwner()->SetActorRotation(NewRotation);
    }
}

// Move the boat forward
void UMovementBoat::MoveForward(float DeltaTime, bool bScurryActive)
{
    FVector DesiredDirection = GetWorldSpaceDirection(MovementInput);
    
    if(CurrentSpeed > 0.0f)
    {
        if(!DesiredDirection.IsNearlyZero())
        {
            if(bScurryActive)
            {
                FVector NewLocation = GetOwner()->GetActorLocation() + (DesiredDirection * CurrentSpeed * ScurryAmount * DeltaTime);
                GetOwner()->SetActorLocation(NewLocation, true);
            }else
            {
                // Apply movement to the boat
                FVector NewLocation = GetOwner()->GetActorLocation() + (DesiredDirection * CurrentSpeed * DeltaTime);
                GetOwner()->SetActorLocation(NewLocation, true);
            }
            
        } else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpringArm is null or input direction is invalid, defaulting to owner forward vector"));
            FVector ForwardDirection = GetOwner()->GetActorForwardVector();
            FVector NewLocation = GetOwner()->GetActorLocation() + (ForwardDirection * CurrentSpeed * DeltaTime);
            GetOwner()->SetActorLocation(NewLocation, true);
        }
    }
}

FVector UMovementBoat::GetWorldSpaceDirection(const FVector2D& InputDirection) const
{
    if(!SpringArm)
    {
        UE_LOG(LogTemp, Error, TEXT("SpringArm is null! Using default forward direction."))
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

void UMovementBoat::FindCameraAndSpringArm()
{
    TeamCamera = nullptr;
    SpringArm = nullptr;

    // Determine the team based on the owner's tags
    FName CameraTag;
    if (GetOwner()->ActorHasTag("BoatPanda")) // OLD VERSION: GetOwner()->ActorHasTag("BoatPanda")
    {
        CameraTag = FName("CameraPanda");
        UE_LOG(LogTemp, Warning, TEXT("Boat belongs to Team Panda."));
    }
    else if (GetOwner()->ActorHasTag("BoatRaccoon")) //GetOwner()->Tags.Find("BoatRaccoon")
    {
        CameraTag = FName("CameraRaccoon");
        UE_LOG(LogTemp, Warning, TEXT("Boat belongs to Team Raccoon."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Boat does not have a valid team tag!"));
        return;
    }
    
    // Locate the correct camera in the world
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor->ActorHasTag(CameraTag))
        {
            TeamCamera = Actor;
            SpringArm = TeamCamera->FindComponentByClass<USpringArmComponent>();
            

            if (!SpringArm)
            {
                UE_LOG(LogTemp, Error, TEXT("SpringArmComponent not found in TeamCamera: %s"), *Actor->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SpringArm successfully found in TeamCamera: %s"), *Actor->GetName());
            }
            return;
        }
    }
}


