// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySpawnPoint.h"

// Sets default values
ALobbySpawnPoint::ALobbySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Arrow Component
	PlayerSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	NameTagSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("NameTagSpawnPoint"));

	// Attach the Arrow Component to the Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PlayerSpawnPoint->SetupAttachment(RootComponent);
	NameTagSpawnPoint->SetupAttachment(RootComponent);

	// Optional: Customize Arrow Appearance
	PlayerSpawnPoint->ArrowColor = FColor::Green;
	PlayerSpawnPoint->SetRelativeScale3D(FVector(1.0f));

	NameTagSpawnPoint->ArrowColor = FColor::Red;
	NameTagSpawnPoint->SetRelativeScale3D(FVector(1.0f));

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


// Spawn the player at the spawn point
void ALobbySpawnPoint::SpawnPlayer(ETeams Team)
{
	if (Team == ETeams::Team_Panda)
	{
		Player = GetWorld()->SpawnActor<AActor>(PandaPlayerClass, PlayerSpawnPoint->GetComponentLocation(), PlayerSpawnPoint->GetComponentRotation());
	} else if (Team == ETeams::Team_Raccoon)
	{
		Player = GetWorld()->SpawnActor<AActor>(RaccoonPlayerClass, PlayerSpawnPoint->GetComponentLocation(), PlayerSpawnPoint->GetComponentRotation());
	}
	
	bIsOccupied = true;
}


// Remove the player from the spawn point
void ALobbySpawnPoint::RemovePlayer()
{
	bIsOccupied = false;

	SteamName = "";
	SteamProfileImage = nullptr;
	Player->Destroy();
}

