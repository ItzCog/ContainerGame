// Copyright Cog


#include "Player/CGPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Core/CGGameMode.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Gameplay/Container.h"
#include "Utility/CGLibrary.h"

// Sets default values
ACGPlayer::ACGPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetSphereRadius(8.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Sphere);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 700.f;
}

void ACGPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (ACGGameMode* ContainerGameMode = UCGLibrary::GetCGGameMode(this))
	{
		ContainerGameMode->OnEndPlacingContainer.AddDynamic(this, &ACGPlayer::OnEndPlacingContainer);
		ContainerGameMode->OnEndBattling.AddDynamic(this, &ACGPlayer::OnEndBattling);
		ContainerGameMode->CanPlaceContainer.BindLambda(
			[this](const AContainer* Container) -> bool
			{
				return Container->GetCost() <= Energy;
			}
			);
	}
}

void ACGPlayer::OnEndPlacingContainer(bool bSuccessful, int32 Cost, AUnit* NewUnit)
{
	if (!bSuccessful) return;

	SetEnergy(Energy - Cost);
}

void ACGPlayer::OnEndBattling(int32 NewTurnNumber)
{
	SetEnergy(NewTurnNumber);
}

