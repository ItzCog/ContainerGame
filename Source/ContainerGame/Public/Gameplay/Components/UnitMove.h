// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMove.generated.h"


class USplineComponent;
class AUnit;

UCLASS( ClassGroup=(UnitBehavior), meta=(BlueprintSpawnableComponent) )
class CONTAINERGAME_API UUnitMove : public UActorComponent
{
	GENERATED_BODY()

public:
	UUnitMove();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(const FVector& Location);

private:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	AUnit* OwnerUnit = nullptr;

	UPROPERTY()
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere)
	float StoppingDistance = 0.1f;

	bool bShouldBeMoving = false;
	FVector MovingTargetLocation;
};
