// Copyright Cog

#include "Gameplay/Unit.h"
#include "Components/CapsuleComponent.h"
#include "Core/CGGameMode.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Utility/CGLibrary.h"

// Sets default values
AUnit::AUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SetRootComponent(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Capsule);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	
}


void AUnit::FinishPlacement_Implementation()
{
	UCGLibrary::GetCGGameMode(this)->AddUnit(this);
	Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	
}

bool AUnit::IsDead() const
{
	return Health <= 0;
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	if (IsNetStartupActor())
	{
		FinishPlacement();
	}
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const int ActualDamage = int(Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser));
	
	Health -= ActualDamage;

	OnHealthChanged.Broadcast(Health);
	return DamageAmount;
}

bool AUnit::IsInAction() const
{
	return bIsInAction;
}

void AUnit::FinishAction()
{
	bIsInAction = false;
}

void AUnit::Move_Implementation()
{
	bIsInAction = true;
}

void AUnit::Attack_Implementation()
{
	bIsInAction = true;
}
