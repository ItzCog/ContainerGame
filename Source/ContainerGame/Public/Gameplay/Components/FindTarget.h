// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FindTarget.generated.h"


class AUnit;

UCLASS( ClassGroup=(UnitBehavior), meta=(BlueprintSpawnableComponent) )
class CONTAINERGAME_API UFindTarget : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFindTarget();

	UFUNCTION(BlueprintCallable)
	AUnit* GetTargetUnit();

protected:
	virtual void BeginPlay() override;

private:
	AUnit* FindTarget();

	bool bIsTargetTemporary = false;
	
	UPROPERTY()
	AUnit* OwnerUnit = nullptr;

	UPROPERTY()
	AUnit* TargetUnit = nullptr;

	UPROPERTY(EditAnywhere)
	float DetectionRadius = 10000.f;
};
