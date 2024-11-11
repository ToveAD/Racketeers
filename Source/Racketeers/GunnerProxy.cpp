#include "GunnerProxy.h"
#include "Net/UnrealNetwork.h"

UGunnerProxy::UGunnerProxy()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true); // Ensure the component is replicated by default
}

void UGunnerProxy::BeginPlay()
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
        UE_LOG(LogTemp, Error, TEXT("Boat not found for GunnerProxy."));
    }
}

void UGunnerProxy::HandleGunnerProxy(float Value)
{
    if (BoatMovementComponent)
    {
        if (!GetOwner()->HasAuthority())
        {
            if (Value >= -1.0f && Value <= 1.0f) // Validate the value on the client side before sending to the server
            {
                ServerHandleGunner(Value); // Request the server to handle the gunner input
            }
        }
        else
        {
            BoatMovementComponent->Steer(Value); // Apply gunner action directly on the server
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BoatMovementComponent is null. Cannot handle gunner input."));
    }
}

void UGunnerProxy::ServerHandleGunner_Implementation(float Value)
{
    if (BoatMovementComponent && (Value >= -1.0f && Value <= 1.0f)) // Validate the value
    {
        BoatMovementComponent->Steer(Value); // Apply gunner action on the server
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid gunner value or BoatMovementComponent is null on server."));
    }
}

bool UGunnerProxy::ServerHandleGunner_Validate(float Value)
{
    return (Value >= -1.0f && Value <= 1.0f); // Ensure input is within the valid range
}

void UGunnerProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UGunnerProxy, Boat);
    DOREPLIFETIME(UGunnerProxy, BoatMovementComponent);
}
