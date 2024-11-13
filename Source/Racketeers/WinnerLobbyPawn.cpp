// Fill out your copyright notice in the Description page of Project Settings.


#include "WinnerLobbyPawn.h"

// Sets default values
AWinnerLobbyPawn::AWinnerLobbyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWinnerLobbyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinnerLobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWinnerLobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

