// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "CGGameMode.h"
#include "GameFramework/WorldSettings.h"
#include "Utility/ContainerSpawnLocationInfo.h"
#include "CGWorldSettings.generated.h"

class ADeployAreaMarker;
class AOpponent;

/**
 * @brief World settings, includes data specific to each level.
 */
UCLASS()
class CONTAINERGAME_API ACGWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	FContainerSpawnLocationInfoHandle FindFirstAvailableLocationForTeamID(int32 TeamID);

	UFUNCTION(BlueprintCallable)
	void UnoccupyLocationForContainer(const AContainer* Container);

	bool IsLocationInsidePlayerDeployArea(const FVector& Location) const;
	
private:
	UPROPERTY(EditAnywhere, Category="Gameplay|Container Spawn")
	TArray<FContainerSpawnLocationInfo> ContainerSpawnLocations;

	UPROPERTY(EditAnywhere, Category="Gameplay|Container Spawn")
	int32 StartingContainerCount = 3;

	UPROPERTY(EditAnywhere, Category="Gameplay|Environment")
	ADeployAreaMarker* DeployAreaMarker;

	UPROPERTY(EditAnywhere, Category="Gameplay|Player")
	AOpponent* Opponent;

public:
	FORCEINLINE int32 GetStartingContainerCount() const { return StartingContainerCount; }
	FORCEINLINE AOpponent* GetOpponent() const { return Opponent; }
};
