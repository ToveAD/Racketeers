// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Lobby.h"
#include "Net/UnrealNetwork.h"


APS_Lobby::APS_Lobby()
{
	bReplicates = true;
}

void APS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Lobby, LobbyInfo);
}
