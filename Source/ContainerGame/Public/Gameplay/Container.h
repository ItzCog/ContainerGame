// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Container.generated.h"

class AUnit;
struct FContainerSpawnLocationInfo;

/**
 * @brief A container in player's hand; has a cost, can be deployed, new ones are added each turn.
 */
UCLASS()
class CONTAINERGAME_API AContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	AContainer();
	void DestroyContainer();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components", BlueprintReadOnly)
	USkeletalMeshComponent* ContainerMesh;

	UPROPERTY(VisibleAnywhere, Category="Components", BlueprintReadOnly)
	USkeletalMeshComponent* UnitPreviewMesh;

	UPROPERTY(EditAnywhere, Category="Gameplay", BlueprintReadOnly)
	TSubclassOf<AUnit> Unit;

	UPROPERTY(EditAnywhere, Category="Gameplay", BlueprintReadOnly)
	int32 TeamID = 0;
	
	UPROPERTY(EditAnywhere, Category="Gameplay", BlueprintReadOnly)
	int32 Cost = 1;

public:
	FORCEINLINE TSubclassOf<AUnit> GetUnit() const { return Unit; }
	FORCEINLINE int32 GetCost() const { return Cost; }
	void SetTeamID(int32 InTeamID) { TeamID = InTeamID; }
};
