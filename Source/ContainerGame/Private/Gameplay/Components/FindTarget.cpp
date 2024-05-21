// Copyright Cog


#include "Gameplay/Components/FindTarget.h"

#include <limits>

#include "Core/CGGameMode.h"
#include "Gameplay/Unit.h"
#include "Kismet/KismetSystemLibrary.h"
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

#if WITH_EDITOR
	if (TargetUnit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Orange,
			FString::Printf(TEXT("%s targets %s"), *OwnerUnit->GetActorLabel(), *TargetUnit->GetActorLabel())
			);
	}
#endif
	
	
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
		FHitResult Hit;
		if (Dist < MinDistance && Dist < DetectionRadius &&
			!UKismetSystemLibrary::LineTraceSingle(this,
				Unit->GetActorLocation(), OwnerUnit->GetActorLocation(),
				TraceTypeQuery32, false, TArray<AActor*>(),
				EDrawDebugTrace::Persistent, Hit, true))
		{
			MinDistance = Dist;
			NewTarget = Unit;
		}
	}

	if (NewTarget) return NewTarget;

	for (AUnit* Unit : UCGLibrary::GetCGGameMode(this)->GetUnits())
	{
		if (!Unit) continue;
		if (!Unit->IsStationary()) continue;;
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

