// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CGPlayer.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UCameraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatsChangeSignature, int32, NewStat);

UCLASS()
class CONTAINERGAME_API ACGPlayer : public APawn
{
	GENERATED_BODY()

public:
	ACGPlayer();

	UPROPERTY(BlueprintAssignable)
	FStatsChangeSignature OnEnergyChanged;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components", BlueprintReadOnly)
	UCameraComponent* Camera;

private:
	UFUNCTION()
	void OnEndPlacingContainer(bool bSuccessful, int32 Cost, AUnit* NewUnit);

	UFUNCTION()
	void OnEndBattling(int32 NewTurnNumber);

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere)
	int32 Energy = 1;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetEnergy() const { return Energy; }

	UFUNCTION(BlueprintCallable)
	void SetEnergy(int32 InEnergy)
	{
		Energy = InEnergy;
		OnEnergyChanged.Broadcast(Energy);
	}
};
