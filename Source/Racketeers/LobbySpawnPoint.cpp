// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySpawnPoint.h"
#include "PC_Lobby.h"
#include "WidgetLobbyInfo.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALobbySpawnPoint::ALobbySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to replicate
	bReplicates = true;
	
	// Create the Arrow Component
	PlayerSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	LobbyInfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// Attach the Arrow Component to the Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PlayerSpawnPoint->SetupAttachment(RootComponent);
	LobbyInfoWidget->SetupAttachment(RootComponent);


	// Optional: Customize Arrow Appearance
	PlayerSpawnPoint->ArrowColor = FColor::Green;
	PlayerSpawnPoint->SetRelativeScale3D(FVector(1.0f));
	
}


// Spawn the player at the spawn point
void ALobbySpawnPoint::SpawnPlayer(APlayerController* PC, ETeams Team)
{
	if (Team == ETeams::Team_Panda)
	{
		Player = GetWorld()->SpawnActor<AActor>(PandaPlayerClass, PlayerSpawnPoint->GetComponentLocation(), PlayerSpawnPoint->GetComponentRotation());
		PlayerController = PC;
		
	} else if (Team == ETeams::Team_Raccoon)
	{
		Player = GetWorld()->SpawnActor<AActor>(RaccoonPlayerClass, PlayerSpawnPoint->GetComponentLocation(), PlayerSpawnPoint->GetComponentRotation());
		PlayerController = PC;
	}

	if(HasAuthority())
	{
		LobbyInfoWidget->SetVisibility(true);
	}
}


// Remove the player from the spawn point
void ALobbySpawnPoint::RemovePlayer()
{
	if(HasAuthority())
	{
		LobbyInfoWidget->SetVisibility(true);
	}
	
	PlayerController = nullptr;
	Player->Destroy();
}

void ALobbySpawnPoint::OnRep_BShowPlayerInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnRep_bShowPlayerInfo"));
	if (bShowPlayerInfo)
	{
		LobbyInfoWidget->SetVisibility(true);
	}
	else
	{
		LobbyInfoWidget->SetVisibility(false);
	}
}

void ALobbySpawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbySpawnPoint, bShowPlayerInfo);
}
