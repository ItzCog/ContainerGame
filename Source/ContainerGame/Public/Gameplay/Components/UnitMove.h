// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMove.generated.h"


class AUnit;

UCLASS( ClassGroup=(UnitBehavior), meta=(BlueprintSpawnableComponent) )
class CONTAINERGAME_API UUnitMove : public UActorComponent
{
	GENERATED_BODY()

public:
	UUnitMove();

	UFUNCTION(BlueprintCallable)
	void Move();

private:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	AUnit* OwnerUnit = nullptr;
};
