// Copyright Cog

#include "Gameplay/Container.h"
#include "Core/CGWorldSettings.h"
#include "Utility/CGLibrary.h"

AContainer::AContainer()
{
	PrimaryActorTick.bCanEverTick = true;

	ContainerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ContainerMesh"));
	SetRootComponent(ContainerMesh);

	UnitPreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnitPreviewMesh"));
	UnitPreviewMesh->SetupAttachment(ContainerMesh);
}

void AContainer::DestroyContainer()
{
	UCGLibrary::GetCGWorldSettings(this)->UnoccupyLocationForContainer(this);
	Destroy();
}

void AContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

