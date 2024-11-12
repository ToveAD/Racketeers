// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"

#include "Net/UnrealNetwork.h"

APS_Base::APS_Base()
{
	// Enable replication for this actor
	bReplicates = true;
	
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate PlayerInfo and each property inside if needed
	DOREPLIFETIME(APS_Base, PlayerInfo);
	DOREPLIFETIME(APS_Base, Team);
	DOREPLIFETIME(APS_Base, TeamPlayerID);
	DOREPLIFETIME(APS_Base, PlayerController);
}