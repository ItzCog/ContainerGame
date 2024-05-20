// Copyright Cog


#include "Gameplay/Components/UnitMove.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Gameplay/Unit.h"

UUnitMove::UUnitMove()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UUnitMove::Move()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(
		OwnerUnit->GetController(),
		OwnerUnit->GetActorLocation() + OwnerUnit->GetActorForwardVector() * 20.f
		);
}

void UUnitMove::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = CastChecked<AUnit>(GetOwner());
}

