// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySpawnPoint.h"

// Sets default values
ALobbySpawnPoint::ALobbySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Arrow Component
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	// Attach the Arrow Component to the Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	// Optional: Customize Arrow Appearance
	ArrowComponent->ArrowColor = FColor::Green;
	ArrowComponent->SetRelativeScale3D(FVector(1.0f));

}

// Called when the game starts or when spawned
void ALobbySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbySpawnPoint::SpawnPlayer(ETeams Team)
{
	if (Team == ETeams::Team_Panda)
	{
		GetWorld()->SpawnActor<AActor>(PandaPlayerClass, ArrowComponent->GetComponentLocation(), ArrowComponent->GetComponentRotation());
	} else if (Team == ETeams::Team_Racoon)
	{
		GetWorld()->SpawnActor<AActor>(RaccoonPlayerClass, ArrowComponent->GetComponentLocation(), ArrowComponent->GetComponentRotation());
	}

	bIsOccupied = true;
}

