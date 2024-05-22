// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "ContainerGame/ContainerGame.h"
#include "GameFramework/GameModeBase.h"
#include "CGGameMode.generated.h"

struct FContainerSpawnLocationInfoHandle;
class AUnit;
class AContainer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefaultPhaseChangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartPlacingContainerSignature, AUnit*, NewUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEndPlacingContainerSignature,
	bool, bSuccessful, int32, Cost, AUnit*, NewUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndBattlingSignature, int32, NewTurnNumber);
DECLARE_DELEGATE_RetVal_OneParam(bool, FCanPlaceContainerSignature, const AContainer*);

/**
 * @brief Handles the fundamental game mechanics, including turn-based logic and commanding units to move and attack.
 */
UCLASS()
class CONTAINERGAME_API ACGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGGameMode();
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void StartPlacingContainer(AContainer* Container);

	UFUNCTION(BlueprintCallable)
	void EndPlacingContainer(bool bSuccessful, AUnit* NewUnit);

	UFUNCTION(BlueprintCallable)
	void StartInspection();

	UFUNCTION(BlueprintCallable)
	void EndInspection();
	
	UFUNCTION(BlueprintCallable)
	void EndPlayerTurn();

	UFUNCTION(BlueprintCallable)
	void EndOpponentTurn();

	UFUNCTION(BlueprintCallable)
	void SpawnContainer(int32 ForTeamID);

	UPROPERTY(BlueprintAssignable)
	FStartPlacingContainerSignature OnStartPlacingContainer;

	UPROPERTY(BlueprintAssignable)
	FEndPlacingContainerSignature OnEndPlacingContainer;

	UPROPERTY(BlueprintAssignable)
	FDefaultPhaseChangeSignature OnStartInspection;

	UPROPERTY(BlueprintAssignable)
	FDefaultPhaseChangeSignature OnEndInspection;

	UPROPERTY(BlueprintAssignable)
	FDefaultPhaseChangeSignature OnStartBattling;

	UPROPERTY(BlueprintAssignable)
	FEndBattlingSignature OnEndBattling;

	UPROPERTY(BlueprintAssignable)
	FDefaultPhaseChangeSignature OnStartOpponentTurn;

	UPROPERTY(BlueprintAssignable)
	FDefaultPhaseChangeSignature OnEndOpponentTurn;
	
	FCanPlaceContainerSignature CanPlaceContainer;

protected:
	virtual void BeginPlay() override;

private:
	void StartBattle();
	
	UPROPERTY(VisibleAnywhere)
	TArray<AUnit*> Units;
	
	EGamePhase GamePhase = EGamePhase::PlayerSelectingContainer;
	int32 TurnNumber = 1;

private:
	enum class EBattlePhase : uint8
	{
		Moving, Attacking,
		HandlingStatusEffects, Cleanup
	};

	void CleanUpDeadUnits();
	void EndTurn();
	void SpawnContainerForClassAndInfo(TSubclassOf<AContainer> ContainerClass, const FContainerSpawnLocationInfoHandle& InfoHandle) const;
	
	EBattlePhase BattlePhase = EBattlePhase::Moving;

	UPROPERTY()
	TArray<TSubclassOf<AContainer>> PlayerContainers;

	UPROPERTY()
	TArray<TSubclassOf<AContainer>> OpponentContainers;
	
	UPROPERTY()
	AContainer* ContainerBeingPlaced;

	int32 CurrentUnitIndex = 0;
	bool bIsCurrentUnitMoving = false;
	bool bHaveUnitsAttacked = false;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGamePhase GetGamePhase() const { return GamePhase; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<AUnit*> GetUnits() const { return Units; }
	
	FORCEINLINE void AddUnit(AUnit* InUnit) { Units.Add(InUnit); }
	
	FORCEINLINE void RemoveUnit(AUnit* InUnit) { Units.Remove(InUnit); }
};
