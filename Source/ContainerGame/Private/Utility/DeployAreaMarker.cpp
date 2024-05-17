// Copyright Cog


#include "Utility/DeployAreaMarker.h"

#include "Components/BoxComponent.h"

ADeployAreaMarker::ADeployAreaMarker()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Box->ShapeColor = FColor::White;
}

bool ADeployAreaMarker::IsLocationInsideArea(const FVector& Location) const
{
	const FVector ActorLocation = GetActorLocation();
	const FVector LocationMin(ActorLocation - Box->GetScaledBoxExtent());
	const FVector LocationMax(ActorLocation + Box->GetScaledBoxExtent());

	return Location.X >= LocationMin.X && Location.X <= LocationMax.X
	&& Location.Y >= LocationMin.Y && Location.Y <= LocationMax.Y
	&& Location.Z >= LocationMin.Z && Location.Z <= LocationMax.Z;
}
