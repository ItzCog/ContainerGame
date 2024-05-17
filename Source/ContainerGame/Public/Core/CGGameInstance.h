// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGGameInstance.generated.h"

class AContainer;

/**
 * @brief Responsible for data persistent across levels, such as the player inventory.
 */
UCLASS()
class CONTAINERGAME_API UCGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TArray<TSubclassOf<AContainer>> ContainerInventory;

public:
	/** Get the array of all containers inside inventory. */
	FORCEINLINE TArray<TSubclassOf<AContainer>> GetContainerInventory() const { return ContainerInventory; }

	/** Add a new container to the player inventory. */
	UFUNCTION(BlueprintCallable)
	void AddToContainerInventory(TSubclassOf<AContainer> NewContainer) { ContainerInventory.Add(NewContainer); }
};
