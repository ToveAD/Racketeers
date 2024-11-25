// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"

#include "OnlineSubsystem.h"
#include "Net/UnrealNetwork.h"

class APlayerState* APS_Base::Duplicate()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "APS_Base::Duplicate()");
	}
	return Super::Duplicate();

}

IOnlineSubsystem

void APS_Base::OverrideWith(APlayerState* PlayerState)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple,  "APS_Base::OverrideWith(PlayerState)");
	Super::OverrideWith(PlayerState);
}

void APS_Base::DispatchPhysicsCollisionHit(const struct FRigidBodyCollisionInfo& MyInfo,
                                           const struct FRigidBodyCollisionInfo& OtherInfo, const FCollisionImpactData& RigidCollisionData)
{
	Super::DispatchPhysicsCollisionHit(MyInfo, OtherInfo, RigidCollisionData);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "APS_Base::DispatchPhysicsCollisionHit");
	}
}

void APS_Base::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "APS_Base::CopyProperties");
	}
}

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
}