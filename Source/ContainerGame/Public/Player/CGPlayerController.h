// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGPlayerController.generated.h"

class AUnit;

/**
 * @brief Player controller, handles player inputs.
 */
UCLASS()
class CONTAINERGAME_API ACGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACGPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float SpeedUpTimeScale = 3.f;
	
	virtual void SetupInputComponent() override;
	void CursorTrace();
	static bool CanBeSelected(const AActor* Actor);

	// Bind to delegates
	UFUNCTION()
	void OnStartPlacingContainer(AUnit* NewUnit);

	UFUNCTION()
	void OnEndPlacingContainer(bool bSuccessful, int32 Cost, AUnit*NewUnit);

	UFUNCTION()
	void OnStartInspection();

	UFUNCTION()
	void OnEndInspection();

	// Input actions
	UFUNCTION()
	void MoveHorizontally(float Value);

	UFUNCTION()
	void MoveVertically(float Value);

	UFUNCTION()
	void Lmb();

	UFUNCTION()
	void Escape();

	UFUNCTION()
	void Inspect();

	UFUNCTION()
	void SpeedUp();

	UFUNCTION()
	void CancelSpeedUp();

	bool bCanMoveVertically = false;
	bool bIsValidPlaceUnitLocation = false;

	UPROPERTY()
	AUnit* UnitBeingPlaced;
};
