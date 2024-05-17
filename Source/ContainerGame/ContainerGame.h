// Copyright Cog

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCG, Display, All);

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	PlayerSelectingContainer,
	PlayerPlacingUnit,
	PlayerInspecting,
	OpponentTurn,
	Battling
};
