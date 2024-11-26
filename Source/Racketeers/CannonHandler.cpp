// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonHandler.h"

// Sets default values for this component's properties
UCannonHandler::UCannonHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCannonHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCannonHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void UCannonHandler::FireCannon(bool ShootLeft, int32 CannonIndex)
//{
	/*
	if(CannonIndex > 0)
	{
		if(ShootLeft)
		{
			if(ProjectileSpawnPoint)
			{
				switch (1)
				{
					
				}
			}
		}else
		{
		
		}
	}
}

void UCannonHandler::ChooseCannon(int32 CannonIndex)
{
	
}
*/



