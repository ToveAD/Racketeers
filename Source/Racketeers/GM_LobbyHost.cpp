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
}

void AGM_LobbyHost::SpawnPlayer(ETeams Team)
{
	
}