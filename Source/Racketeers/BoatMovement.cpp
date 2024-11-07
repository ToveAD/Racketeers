#include "BoatMovement.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"  // Ensure you include this header for input context management
#include "EnhancedInputSubsystems.h" // For Enhanced Input Subsystem to access input contexts

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
        BoatMesh->SetIsReplicated(true); // Enable replication for the boat mesh
        //BoatMesh->GetOwner()->SetReplicateMovement(true); // Enable movement replication
    }

    // Load input mapping contexts during BeginPlay, once assets are properly initialized
    IMC_Boat = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Racketeers/Characters/Input/IMC_Boat.IMC_Boat"));
    IMC_Default = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Racketeers/Characters/Input/IMC_Default.IMC_Default"));

    // Log to check if the loading was successful
    if (!IMC_Boat)
    {
        UE_LOG(LogTemp, Warning, TEXT("IMC_Boat failed to load!"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("IMC_Boat loaded successfully!"));
    }

    if (!IMC_Default)
    {
        UE_LOG(LogTemp, Warning, TEXT("IMC_Default failed to load!"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("IMC_Default loaded successfully!"));
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
        // Move the boat to the new location using the Teleport flag to avoid physics interference
        BoatMesh->SetWorldLocation(NewLocation, true);  // Teleport flag is set to true to move without physics collision
    }
}

// Function to switch input mappings to "IMC_Boat" when the boat is entered
void UBoatMovement::SwitchToBoatInputMapping(bool IsAttaching)
{
    // Get the local player controller
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (PlayerController)
    {
        // Ensure the player controller has an enhanced input subsystem
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (InputSubsystem)
        {
            if (IMC_Boat)  // Check if IMC_Boat is valid
            {
                if(IsAttaching)
                {
                    // Remove the default input mapping context
                    InputSubsystem->RemoveMappingContext(IMC_Default);

                    // Add the boat-specific input mapping context
                    InputSubsystem->AddMappingContext(IMC_Boat, 1);  // You can set priority if needed (0 is default)

                    UE_LOG(LogTemp, Log, TEXT("Switched to boat input mapping context: IMC_Boat"));
                    
                }else if(!IsAttaching)
                {
                    // Remove the default input mapping context
                    InputSubsystem->RemoveMappingContext(IMC_Boat);

                    // Add the boat-specific input mapping context
                    InputSubsystem->AddMappingContext(IMC_Default, 1);  // You can set priority if needed (0 is default)

                    UE_LOG(LogTemp, Log, TEXT("Switched to Player input mapping context: IMC_Default"));
                }
               
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("IMC_Boat is null!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Input subsystem not found for player controller."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
    }
}
