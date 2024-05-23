// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeployAreaMarker.generated.h"

class UBoxComponent;

/**
 * @brief Used to mark the area where the player can place their container.
 */
UCLASS()
class CONTAINERGAME_API ADeployAreaMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeployAreaMarker();
	bool IsLocationInsideArea(const FVector& Location) const;

private:
	UPROPERTY(EditAnywhere, Category="Components")
	UBoxComponent* Box;
};
