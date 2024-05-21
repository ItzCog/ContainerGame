// Copyright Cog


#include "Gameplay/Components/UnitMove.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Gameplay/Unit.h"

UUnitMove::UUnitMove()
{
	PrimaryComponentTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->ClearSplinePoints();
}

void UUnitMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShouldBeMoving) return;

	const FVector MoveDirection =
				Spline->FindDirectionClosestToWorldLocation(OwnerUnit->GetActorLocation(),
															ESplineCoordinateSpace::World);
	OwnerUnit->AddMovementInput(MoveDirection);
	if (FVector::Dist(OwnerUnit->GetActorLocation(), MovingTargetLocation) <= StoppingDistance)
	{
		bShouldBeMoving = false;
	}
}

void UUnitMove::MoveToLocation(const FVector& Location)
{
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this,
		OwnerUnit->GetActorLocation(), Location);

	if (Path)
	{
		Spline->ClearSplinePoints();
		TArray<FVector> PathPoints = Path->PathPoints;
		for (const FVector& Point : PathPoints)
		{
			Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World);
		}

		bShouldBeMoving = true;
		MovingTargetLocation = PathPoints[PathPoints.Num() - 1];
	}
}

void UUnitMove::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = CastChecked<AUnit>(GetOwner());
}

