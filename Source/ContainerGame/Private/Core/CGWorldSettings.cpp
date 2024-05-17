// Copyright Cog

#include "Core/CGWorldSettings.h"
#include "ContainerGame/ContainerGame.h"
#include "Core/CGGameMode.h"
#include "Utility/ContainerSpawnLocationInfo.h"

FContainerSpawnLocationInfoHandle ACGWorldSettings::FindFirstAvailableLocationForTeamID(int32 TeamID)
{
	for (FContainerSpawnLocationInfo& Info : ContainerSpawnLocations)
	{
		if (!Info.GetOccupyingContainer() && Info.GetTeamID() == TeamID)
		{
			return FContainerSpawnLocationInfoHandle(Info);
		}
	}
	return FContainerSpawnLocationInfoHandle::Invalid;
}

void ACGWorldSettings::UnoccupyLocationForContainer(const AContainer* Container)
{
	for (FContainerSpawnLocationInfo& Info : ContainerSpawnLocations)
	{
		if (Info.GetOccupyingContainer() == Container)
		{
			Info.SetOccupyingContainer(nullptr);
		}
	}

	UE_LOG(LogCG, Warning, TEXT("Attempted to unoccupy for a container that does not have an occupied location"));
}
