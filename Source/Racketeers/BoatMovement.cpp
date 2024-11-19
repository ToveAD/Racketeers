#include "BoatMovement.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UBoatMovement::UBoatMovement()
{
    PrimaryComponentTick.bCanEverTick = true; // Enable ticking for this component
}

void UBoatMovement::BeginPlay()
{
    Super::BeginPlay();

    // Get the StaticMesh or any other PrimitiveComponent that simulates physics
    BoatMesh = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));

    if (BoatMesh)
    {
       // BoatMesh->SetSimulatePhysics(true);  // Ensure that physics simulation is enabled
        //BoatMesh->SetIsReplicated(true);     // Enable replication for the boat mesh
       // BoatMesh->GetOwner()->SetReplicateMovement(true); // Enable movement replication
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
    
}

void UBoatMovement::Accelerate(float Value)
{
    
    if (BoatMesh)
    {
        FVector ForwardVector = GetOwner()->GetActorForwardVector();
        ForwardVector.Z = 0.0f;

        FVector ForwardForce = ForwardVector * Value * BoatSpeed;
        BoatMesh->AddForce(ForwardForce, NAME_None, true);

        // Log the applied force for debugging
        UE_LOG(LogTemp, Warning, TEXT("Server Boat: %s, ForwardForce: %s, BoatSpeed: %f"),
               *GetOwner()->GetName(), *ForwardForce.ToString(), BoatSpeed);
    }
    
}

void UBoatMovement::Steer(float Value)
{
        if (BoatMesh)
        {
            // Calculate torque to apply for steering
            FVector Torque = FVector(0.0f, 0.0f, Value * SteeringSpeed);
            BoatMesh->AddTorqueInDegrees(Torque, NAME_None, true); // Apply torque to the boat mesh
        }
}

void UBoatMovement::TeleportBoat(const FVector& NewLocation)
{
    if (BoatMesh)
    {
        BoatMesh->SetWorldLocation(NewLocation, true); // Teleport the boat to the new location without collision issues
    }
}

void UBoatMovement::SwitchInputMapping(bool IsAttaching, UInputMappingContext* InputToAdd, UInputMappingContext* InputToRemove, APlayerController* PlayerController)
{
    //APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); // Get the player controller

    if (PlayerController)
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) // Get the local player
        {
            UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

            if (InputSubsystem)
            {
                if (InputToAdd && InputToRemove)
                {
                    if (IsAttaching)
                    {
                        // Switch to boat input mapping when attaching
                        InputSubsystem->RemoveMappingContext(InputToRemove);
                        InputSubsystem->AddMappingContext(InputToAdd, 1);
                        UE_LOG(LogTemp, Log, TEXT("Switched to boat input mapping context: IMC_Boat"));
                    }
                    else
                    {
                        // Switch back to default input mapping when detaching
                        InputSubsystem->RemoveMappingContext(InputToAdd);
                        InputSubsystem->AddMappingContext(InputToRemove, 1);
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
            UE_LOG(LogTemp, Warning, TEXT("LocalPlayer not found for player controller."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
    }
}

