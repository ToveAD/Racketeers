// Fill out your copyright notice in the Description page of Project Settings.


#include "RoleMovement.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"

// Sets default values for this component's properties
URoleMovement::URoleMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoleMovement::BeginPlay()
{
	Super::BeginPlay();

	// Get the owner of this component (e.g., BP_Ship)
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// Find and assign the TurretMesh component by class or name
		TurretMesh = Owner->FindComponentByClass<UStaticMeshComponent>();
        
		// If needed, you can also find by name (replace "TurretMesh" with the actual name in BP_Ship)
		// TurretMesh = Cast<UStaticMeshComponent>(Owner->GetComponentByName(TEXT("TurretMesh")));

		// Find and assign the ProjectileSpawnPoint component by class or name
		ProjectileSpawnPoint = Owner->FindComponentByClass<USceneComponent>();

		// Log a warning if they were not found
		if (!TurretMesh || !ProjectileSpawnPoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("TurretMesh or ProjectileSpawnPoint not found on %s"), *Owner->GetName());
		}
	}
	
}


// Called every frame
void URoleMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateTurretTowardsMouse();

	// ...
}

void URoleMovement::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

	TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 20));
}

void URoleMovement::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	//DrawDebugSphere(GetWorld(),Location,25,12,FColor::Red, false, 3);
	
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	if (AActor* OwnerActor = GetOwner())
	{
		Projectile->SetOwner(OwnerActor);
	}
}

void URoleMovement::RotateTurretTowardsMouse()
{
	// Get the player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController || !TurretMesh) return;

	// Get the mouse position in screen space
	float MouseX, MouseY;
	if (PlayerController->GetMousePosition(MouseX, MouseY))
	{
		// Convert mouse position to world direction
		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
		{
			// Define a point far along the direction to ensure it intersects with world space
			FVector EndPoint = WorldLocation + (WorldDirection * 10000.0f);

			// Find where the mouse intersects with a ground plane or another object
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner()); // Ignore the owning ship to prevent self-intersections

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndPoint, ECC_Visibility, Params))
			{
				FVector TargetLocation = HitResult.Location;

				// Calculate the look-at rotation for the turret
				FVector ToTarget = TargetLocation - TurretMesh->GetComponentLocation();
				FRotator TargetRotation = FRotator(0, ToTarget.Rotation().Yaw, 0);

				// Smoothly rotate turret towards target
				TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5.0f));
			}
		}
	}
}

