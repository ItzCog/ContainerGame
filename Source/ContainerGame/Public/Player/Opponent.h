// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Opponent.generated.h"

class AContainer;

UCLASS()
class CONTAINERGAME_API AOpponent : public AActor
{
	GENERATED_BODY()
	
public:	
	AOpponent();

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category="Gameplay")
	TArray<TSubclassOf<AContainer>> Inventory;

public:
	FORCEINLINE TArray<TSubclassOf<AContainer>> GetInventory() const { return Inventory; }
};
