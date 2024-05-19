// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FindTarget.generated.h"


class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	
	UPROPERTY()
	AUnit* OwnerUnit = nullptr;

	UPROPERTY()
	AUnit* TargetUnit = nullptr;
};
