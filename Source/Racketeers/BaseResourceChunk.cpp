// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseResourceChunk.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ABaseResourceChunk::ABaseResourceChunk()
{
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create and initialize the Static Mesh Component
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	RootComponent = ResourceMesh;

	//Create and initialize the Projectile Movement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 10000.0f;

	bReplicates = true;
	
}

// Sets default values
ABaseResourceChunk::ABaseResourceChunk(FVector initialVelocity): ABaseResourceChunk()
{
	velocity = initialVelocity;
	
}

// Called when the game starts or when spawned
void ABaseResourceChunk::BeginPlay()
{
	Super::BeginPlay();
	
	// if(!velocity.IsZero())
	// 	ProjectileMovement->Velocity = velocity*throwForce;
	// else
	// 	ProjectileMovement->Velocity = GetActorUpVector()* throwForce;
}

void ABaseResourceChunk::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseResourceChunk, LastOwner);
}

// Called every frame
void ABaseResourceChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseResourceChunk::GroundCheck_Implementation()
{
	if(!GetWorld()) return false;

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 25.0f);

	FHitResult HitResult;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		FCollisionQueryParams(FName(TEXT("FloorCheck")), false, this)
	);

	if(bHit && IsValid(HitResult.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Floor: %s"), *HitResult.GetActor()->GetName());
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not grounded"));
		return false;
	}

}

