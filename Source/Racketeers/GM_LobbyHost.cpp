// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_LobbyHost.h"

#include "LobbySpawnPoint.h"
#include "PC_Lobby.h"
#include "Kismet/GameplayStatics.h"

void AGM_LobbyHost::BeginPlay()
{
	Super::BeginPlay();
	SetUpSpawnPositions();
}

void AGM_LobbyHost::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (APC_Lobby* PlayerController = Cast<APC_Lobby>(NewPlayer))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Player Joined"));
		PlayerController->RequestTeamSelection();
	}
}

void AGM_LobbyHost::OnLogout(AController* Exiting)
{
}

void AGM_LobbyHost::SetUpSpawnPositions()
{
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALobbySpawnPoint::StaticClass(), TEXT("Panda"), PandaPositions);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALobbySpawnPoint::StaticClass(), TEXT("Raccoon"), RaccoonPositions);


	// Sort the spawn points by TeamID
	PandaPositions.Sort([](const AActor& A, const AActor& B)
	{
		const ALobbySpawnPoint* SpawnPointA = Cast<ALobbySpawnPoint>(&A);
		const ALobbySpawnPoint* SpawnPointB = Cast<ALobbySpawnPoint>(&B);

		if (SpawnPointA && SpawnPointB)
		{
			return SpawnPointA->TeamID < SpawnPointB->TeamID;
		}
		return false;
	});

	RaccoonPositions.Sort([](const AActor& A, const AActor& B)
	{
		const ALobbySpawnPoint* SpawnPointA = Cast<ALobbySpawnPoint>(&A);
		const ALobbySpawnPoint* SpawnPointB = Cast<ALobbySpawnPoint>(&B);

		if (SpawnPointA && SpawnPointB)
		{
			return SpawnPointA->TeamID < SpawnPointB->TeamID;
		}
		return false;
	});
}

// Spawn the player at the first available spawn point and set spawn point in player controller
void AGM_LobbyHost::SpawnPlayer(APlayerController* PC, ETeams Team)
{
	if (Team == ETeams::Team_Panda)
	{
		for (auto Position : PandaPositions)
		{
			if(Cast<ALobbySpawnPoint>(Position)->bIsOccupied == false)
			{
				ALobbySpawnPoint* SP = Cast<ALobbySpawnPoint>(Position);
				SP->SpawnPlayer(Team);
				Cast<APC_Lobby>(PC)->SpawnPoint = SP;
				return;
			}
		}
	} else if (Team == ETeams::Team_Racoon)
	{
		for (auto Position : RaccoonPositions)
		{
			if (Cast<ALobbySpawnPoint>(Position)->bIsOccupied == false)
			{
				ALobbySpawnPoint* SP = Cast<ALobbySpawnPoint>(Position);
				SP->SpawnPlayer(Team);
				Cast<APC_Lobby>(PC)->SpawnPoint = SP;
				return;
			}
		}
	}
	
}