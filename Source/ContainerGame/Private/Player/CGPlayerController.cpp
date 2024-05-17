// Copyright Cog


#include "Player/CGPlayerController.h"
#include "Utility/CGLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Core/CGGameMode.h"
#include "Gameplay/Unit.h"
#include "Kismet/GameplayStatics.h"

ACGPlayerController::ACGPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	
	PrimaryActorTick.bCanEverTick = true;
	UnitBeingPlaced = nullptr;
}

void ACGPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	if (UnitBeingPlaced)
	{
		const float UnitHalfHeight = UnitBeingPlaced->GetCapsule()->GetScaledCapsuleHalfHeight();
		
		FHitResult HitResult, UnitHitResult;
		
		GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);
		GetHitResultUnderCursor(ECC_GameTraceChannel2, false, UnitHitResult);

		if (HitResult.bBlockingHit && !UnitHitResult.bBlockingHit
			&& HitResult.GetActor()->ActorHasTag(FName("BattleArea")))
		{
			UnitBeingPlaced->SetActorLocation(HitResult.ImpactPoint + FVector::UpVector * UnitHalfHeight);
			bIsValidPlaceUnitLocation = true;
		}
		else
		{
			bIsValidPlaceUnitLocation = false;
		}
	}
	else
	{
		CursorTrace();
	}
}

void ACGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ACGGameMode* ContainerGameMode = UCGLibrary::GetCGGameMode(this))
	{
		ContainerGameMode->OnStartPlacingContainer.AddDynamic(this,
			&ACGPlayerController::OnStartPlacingContainer);
		ContainerGameMode->OnEndPlacingContainer.AddDynamic(this,
			&ACGPlayerController::OnEndPlacingContainer);
		ContainerGameMode->OnStartInspection.AddDynamic(this,
			&ACGPlayerController::OnStartInspection);
		ContainerGameMode->OnEndInspection.AddDynamic(this,
			&ACGPlayerController::OnEndInspection);
	}
}

void ACGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("MoveHorizontal", this, &ACGPlayerController::MoveHorizontally);
	InputComponent->BindAxis("MoveVertical", this, &ACGPlayerController::MoveVertically);
	InputComponent->BindAction("Lmb",  IE_Pressed, this, &ACGPlayerController::Lmb);
	InputComponent->BindAction("Escape",  IE_Pressed, this, &ACGPlayerController::Escape);
	InputComponent->BindAction("Inspect",  IE_Pressed, this, &ACGPlayerController::Inspect);
	InputComponent->BindAction("SpeedUp",  IE_Pressed, this, &ACGPlayerController::SpeedUp);
	InputComponent->BindAction("SpeedUp",  IE_Released, this, &ACGPlayerController::CancelSpeedUp);
}

void ACGPlayerController::CursorTrace()
{
}

bool ACGPlayerController::CanBeSelected(const AActor* Actor)
{
	return false;
}

void ACGPlayerController::MoveHorizontally(float Value)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	ControlledPawn->AddMovementInput(FVector::RightVector, Value);
}

void ACGPlayerController::MoveVertically(float Value)
{
	if (!bCanMoveVertically) return;
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	
	ControlledPawn->AddMovementInput(FVector::ForwardVector, Value);
}

void ACGPlayerController::Lmb()
{
	if (UCGLibrary::GetCGGameMode(this)->GetGamePhase()
		== EGamePhase::PlayerSelectingContainer)
	{
	}
	else
	{
		if (UnitBeingPlaced && bIsValidPlaceUnitLocation)
		{
			UnitBeingPlaced->FinishPlacement();
			UCGLibrary::GetCGGameMode(this)
				->EndPlacingContainer(true, UnitBeingPlaced);
			UnitBeingPlaced = nullptr;
		}
	}
}

void ACGPlayerController::Escape()
{
	ACGGameMode* ContainerGameMode = UCGLibrary::GetCGGameMode(this);

	if (!ContainerGameMode) return;
	
	if (UnitBeingPlaced)
	{
		UnitBeingPlaced->Destroy();
		UnitBeingPlaced = nullptr;
		ContainerGameMode->EndPlacingContainer(false, nullptr);
	}
	else if (ContainerGameMode->GetGamePhase() == EGamePhase::PlayerInspecting)
	{
		ContainerGameMode->EndInspection();
	}
}

void ACGPlayerController::Inspect()
{
	ACGGameMode* ContainerGameMode = UCGLibrary::GetCGGameMode(this);
	if (!ContainerGameMode) return;
	
	if (ContainerGameMode->GetGamePhase() == EGamePhase::PlayerSelectingContainer)
	{
		ContainerGameMode->StartInspection();
	}
	else if (ContainerGameMode->GetGamePhase() == EGamePhase::PlayerInspecting)
	{
		ContainerGameMode->EndInspection();
	}
}

void ACGPlayerController::SpeedUp()
{
	UGameplayStatics::SetGlobalTimeDilation(this, SpeedUpTimeScale);
}

void ACGPlayerController::CancelSpeedUp()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);
}

void ACGPlayerController::OnStartPlacingContainer(AUnit* NewUnit)
{
	bCanMoveVertically = true;
	UnitBeingPlaced = NewUnit;
}

void ACGPlayerController::OnEndPlacingContainer(bool bSuccessful, int32 Cost, AUnit*NewUnit)
{
	bCanMoveVertically = false;
}

void ACGPlayerController::OnStartInspection()
{
	bCanMoveVertically = true;
}

void ACGPlayerController::OnEndInspection()
{
	bCanMoveVertically = false;
}
