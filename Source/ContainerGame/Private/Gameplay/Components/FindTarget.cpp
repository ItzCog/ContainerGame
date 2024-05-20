// Copyright Cog


#include "Gameplay/Components/FindTarget.h"

#include <limits>

#include "Core/CGGameMode.h"
#include "Gameplay/Unit.h"
#include "Utility/CGLibrary.h"

UFindTarget::UFindTarget()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

AUnit* UFindTarget::GetTargetUnit()
{
	if (!TargetUnit)
	{
		TargetUnit = FindTarget();
	}

	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Orange,
		FString::Printf(TEXT("%s targets %s"), *OwnerUnit->GetActorLabel(), *TargetUnit->GetActorLabel())
		);
	
	return TargetUnit;
}

void UFindTarget::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = CastChecked<AUnit>(GetOwner());
}

AUnit* UFindTarget::FindTarget()
{
	float MinDistance = std::numeric_limits<float>::max();
	AUnit* NewTarget = nullptr;

	for (AUnit* Unit : UCGLibrary::GetCGGameMode(this)->GetUnits())
	{
		if (!Unit) continue;
		if (Unit->GetTeamID() == OwnerUnit->GetTeamID()) continue;

		const float Dist = FVector::Dist(Unit->GetActorLocation(), OwnerUnit->GetActorLocation());
		if (Dist < MinDistance)
		{
			MinDistance = Dist;
			NewTarget = Unit;
		}
	}
	
	return NewTarget;
}

