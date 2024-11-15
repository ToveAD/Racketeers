#include "DriverAccelerateProxy.h"
#include "Net/UnrealNetwork.h"

UDriverAccelerateProxy::UDriverAccelerateProxy()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true); // Ensure the component is replicated by default
}

void UDriverAccelerateProxy::BeginPlay()
{
    Super::BeginPlay();
    Boat = GetOwner();

    // Get the BoatMovement component from the boat actor
    if (Boat)
    {
        BoatMovementComponent = Boat->FindComponentByClass<UBoatMovement>();
        if (!BoatMovementComponent)
        {
            UE_LOG(LogTemp, Error, TEXT("BoatMovementComponent not found on %s."), *Boat->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Boat not found for DriverAccelerateProxy."));
    }
}

void UDriverAccelerateProxy::HandleAccelerationInput(float Value)
{
    //if (BoatMovementComponent)
    //{
     //  if (!GetOwner()->HasAuthority())
       // {
         //   if (Value >= -1.0f && Value <= 1.0f) // Validate the value on the client side before sending to the server
           // {
             //   ServerHandleAcceleration(Value); // Request the server to handle acceleration
            ///}
        //}
        //else
       // {
          //  BoatMovementComponent->Accelerate(Value, DeltaTime); // Apply acceleration directly on the server
        //}
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("BoatMovementComponent is null. Cannot handle acceleration input."));
    //}
}

void UDriverAccelerateProxy::ServerHandleAcceleration_Implementation(float Value)
{
    if (BoatMovementComponent && (Value >= -1.0f && Value <= 1.0f)) // Validate the value
    {
       // BoatMovementComponent->Accelerate(Value); // Apply acceleration on the server
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid acceleration value or BoatMovementComponent is null on server."));
    }
}

bool UDriverAccelerateProxy::ServerHandleAcceleration_Validate(float Value)
{
    return (Value >= -1.0f && Value <= 1.0f); // Ensure input is within the valid range
}

void UDriverAccelerateProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UDriverAccelerateProxy, Boat);
    DOREPLIFETIME(UDriverAccelerateProxy, BoatMovementComponent);
}
