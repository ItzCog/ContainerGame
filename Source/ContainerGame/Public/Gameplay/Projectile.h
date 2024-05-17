// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileDestroySignature);

/**
 * @brief A projectile, typically spawned by a unit.
 */
UCLASS()
class CONTAINERGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnProjectileDestroySignature OnProjectileDestroy;

	UFUNCTION(BlueprintCallable)
	void InitProjectile(int32 InTeamID);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	int32 BaseDamage = 1;

	int32 TeamID;
};
