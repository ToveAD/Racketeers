#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoatSettings.generated.h"
//#include "FBoatSettings.h"

USTRUCT(BlueprintType)
struct FBoatSettings
{
	GENERATED_BODY()

	// Number of cannons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	int32 CannonAmount;

	// Material for the sails
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	UMaterialInterface* SailMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Boat Settings")
	AActor* HullMesh;

	// Movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	float MovementSpeed;

	// Rotation speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	float RotationSpeed;

	// Deceleration rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Settings")
	float DecelerationRate;

	// Default constructor
	FBoatSettings()
		: CannonAmount(0)
		, SailMaterial(nullptr)
		, HullMesh(nullptr)
		, MovementSpeed(0.0f)
		, RotationSpeed(0.0f)
		, DecelerationRate(0.0f)
	{
	}
};