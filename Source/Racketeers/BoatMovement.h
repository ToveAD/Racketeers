#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatMovement.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJ_TEST_API UBoatMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	UBoatMovement();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Input methods to move and steer the boat
    UFUNCTION(BlueprintCallable, Category = "Boat Movement")
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Boat Movement")
	void Steer(float Value);

private:
	// Movement parameters
	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float BoatSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float TurnSpeed = 100.0f;

	// References to important components
	UPrimitiveComponent* BoatMesh;

	float CurrentForwardInput;
	float CurrentSteerInput;
};
