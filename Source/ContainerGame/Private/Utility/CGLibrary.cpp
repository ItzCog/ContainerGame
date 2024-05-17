// Copyright Cog


#include "Utility/CGLibrary.h"

#include "Core/CGGameInstance.h"
#include "Core/CGGameMode.h"
#include "Core/CGWorldSettings.h"
#include "Gameplay/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CGPlayer.h"
#include "Player/CGPlayerController.h"

ACGGameMode* UCGLibrary::GetCGGameMode(const UObject* WorldContextObject)
{
	return Cast<ACGGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

ACGPlayer* UCGLibrary::GetCGPlayer(const UObject* WorldContextObject)
{
	return Cast<ACGPlayer>(UGameplayStatics::GetPlayerPawn(WorldContextObject, 0));
}

ACGPlayerController* UCGLibrary::GetCGPlayerController(const UObject* WorldContextObject)
{
	return Cast<ACGPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

ACGWorldSettings* UCGLibrary::GetCGWorldSettings(const UObject* WorldContextObject)
{
	return WorldContextObject && WorldContextObject->GetWorld()
		? Cast<ACGWorldSettings>(WorldContextObject->GetWorld()->GetWorldSettings())
		: nullptr;
}

UCGGameInstance* UCGLibrary::GetCGGameInstance(const UObject* WorldContextObject)
{
	return Cast<UCGGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

AProjectile* UCGLibrary::SpawnProjectile(const UObject* WorldContextObject, TSubclassOf<AProjectile> ProjectileClass,
	const FTransform& Transform, int32 TeamID, APawn* Instigator)
{
	if (!WorldContextObject) return nullptr;
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	AProjectile* Projectile =
		World->SpawnActorDeferred<AProjectile>(ProjectileClass, Transform, Instigator, Instigator);
	if (!Projectile) return nullptr;

	Projectile->InitProjectile(TeamID);
	Projectile->FinishSpawning(Transform);
	return Projectile;
}
