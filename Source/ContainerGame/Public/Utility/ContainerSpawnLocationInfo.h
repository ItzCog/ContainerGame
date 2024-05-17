// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "ContainerSpawnLocationInfo.generated.h"

class AContainer;

/**
 * @brief Class for marking information used by the Game Mode to spawn the containers at the start of the battle
 * and after end of each round.
 */
USTRUCT()
struct FContainerSpawnLocationInfo
{
	GENERATED_BODY()

public:
	FORCEINLINE int32 GetTeamID() const { return TeamID; }
	FORCEINLINE const FVector& GetSpawnLocation() const { return SpawnLocation; }
	FORCEINLINE const AContainer* GetOccupyingContainer() const { return OccupyingContainer; }
	void SetOccupyingContainer(AContainer* Container) { OccupyingContainer = Container; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 TeamID = 0;

	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;
	
	UPROPERTY(EditAnywhere)
	AContainer* OccupyingContainer = nullptr;
};

/**
 * @brief Handle for FContainerSpawnLocationInfo.
 */
USTRUCT()
struct FContainerSpawnLocationInfoHandle
{
	GENERATED_BODY()
	
public:
	explicit FContainerSpawnLocationInfoHandle(FContainerSpawnLocationInfo& ContainerSpawnLocationInfo)
		: Info(&ContainerSpawnLocationInfo) { }
	FContainerSpawnLocationInfoHandle() : Info(nullptr) {}

	static FContainerSpawnLocationInfoHandle Invalid;

	FContainerSpawnLocationInfo* Get() const { return Info; }
	bool IsValid() const { return Info != nullptr; }
	
private:
	FContainerSpawnLocationInfo* Info;
};