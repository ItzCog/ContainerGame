// Copyright Cog


#include "Gameplay/Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Gameplay/Unit.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SetRootComponent(Sphere);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;

	Tags.Add(FName("Projectile"));
}

void AProjectile::InitProjectile(int32 InTeamID)
{
	TeamID = InTeamID;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::ProjectileImpact);
}

void AProjectile::ProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Projectile"))) return;
	
	const AUnit* OtherUnit = Cast<AUnit>(OtherActor);
	if (OtherUnit && OtherUnit->GetTeamID() == TeamID) return;
	
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigator()->GetController(),
		this, UDamageType::StaticClass());

	Destroy();
	OnProjectileDestroy.Execute();
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

