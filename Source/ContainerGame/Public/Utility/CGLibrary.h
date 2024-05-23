// Copyright Cog

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Projectile.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGLibrary.generated.h"

class AProjectile;
class UCGGameInstance;
class ACGWorldSettings;
class ACGPlayerController;
class ACGPlayer;
class ACGGameMode;

/**
 * @brief Blueprint function library for obtaining important class instances of the game.
 */
UCLASS()
class CONTAINERGAME_API UCGLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static ACGGameMode* GetCGGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static ACGPlayer* GetCGPlayer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static ACGPlayerController* GetCGPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static ACGWorldSettings* GetCGWorldSettings(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static UCGGameInstance* GetCGGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static AProjectile* SpawnProjectile(const UObject* WorldContextObject, TSubclassOf<AProjectile> ProjectileClass,
	                                    const FTransform& Transform, int32 TeamID, APawn* Instigator,
	                                    const FOnProjectileDestroySignature& OnProjectileDestroyedDelegate);
	
	template<typename T>
	static void ShuffleArray(TArray<T>& Array);
};

template <typename T>
void UCGLibrary::ShuffleArray(TArray<T>& Array)
{
	const int32 LastIndex = Array.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			Array.Swap(i, Index);
		}
	}
}
