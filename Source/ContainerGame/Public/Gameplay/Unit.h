// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;

/**
 * @brief A unit deployed on board; Capable of moving and attacking, has health, etc.
 */
UCLASS()
class CONTAINERGAME_API AUnit : public APawn
{
	GENERATED_BODY()

public:
	AUnit();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	bool IsInAction() const;

	UFUNCTION(BlueprintCallable)
	void FinishAction();

	UFUNCTION(BlueprintNativeEvent, Category="Gameplay", BlueprintCallable)
	void Move();

	UFUNCTION(BlueprintNativeEvent, Category="Gameplay", BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintNativeEvent, Category="Gameplay", BlueprintCallable)
	void FinishPlacement();

	bool IsDead() const;

	UPROPERTY(EditAnywhere, Category="Gameplay")
	bool bCanBeTargeted = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components", BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category="Gameplay", BlueprintReadWrite)
	int32 MaxHealth = 1;

	UPROPERTY(Category="Gameplay", BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, Category="Gameplay", BlueprintReadWrite)
	int32 TeamID = 0;

private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditAnywhere, Category="Gameplay")
	bool bIsStationary;
	
	bool bIsInAction = false;

public:
	FORCEINLINE UCapsuleComponent* GetCapsule() const { return Capsule; }
	FORCEINLINE int32 GetTeamID() const { return TeamID; }
	FORCEINLINE bool IsStationary() const { return bIsStationary; }
};
