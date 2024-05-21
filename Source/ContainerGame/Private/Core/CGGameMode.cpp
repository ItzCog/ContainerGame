// Copyright Cog


#include "Core/CGGameMode.h"
#include "Utility/CGLibrary.h"
#include "Core/CGGameInstance.h"
#include "Core/CGWorldSettings.h"
#include "Gameplay/Container.h"
#include "Player/Opponent.h"
#include "Gameplay/Unit.h"
#include "Utility/ContainerSpawnLocationInfo.h"

ACGGameMode::ACGGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	ContainerBeingPlaced = nullptr;
}

void ACGGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GamePhase != EGamePhase::Battling) return;
	
	switch (BattlePhase)
	{
	case EBattlePhase::Moving:
		if (CurrentUnitIndex == Units.Num())
		{
			CurrentUnitIndex = 0;
			BattlePhase = EBattlePhase::Attacking;
		}
		else
		{
			AUnit* CurrentUnit = Units[CurrentUnitIndex];

			// TODO: fix the issue that CurrentUnit could be null
			if (!bIsCurrentUnitMoving)
			{
				CurrentUnit->Move();
				bIsCurrentUnitMoving = true;
			}
			if (!CurrentUnit->IsInAction())
			{
				++CurrentUnitIndex;
				bIsCurrentUnitMoving = false;
			}
		}
		break;

	case EBattlePhase::Attacking:
		if (!bHaveUnitsAttacked)
		{
			bHaveUnitsAttacked = true;
			for (AUnit* Unit : Units)
			{
				Unit->Attack();
			}
		}
		
		if (CurrentUnitIndex == Units.Num())
		{
			for (AUnit* Unit : Units)
			{
				if (!Unit || Unit->IsActorBeingDestroyed()) continue;
				if (Unit->IsDead())
				{
					Unit->Destroy();
				}
			}
			
			Units.RemoveAll([](const AUnit* Unit)->bool
			{
				return Unit == nullptr;
			}); 
			EndTurn();
		}
		else
		{
			const AUnit* CurrentUnit = Units[CurrentUnitIndex];
			if (!CurrentUnit->IsInAction())
			{
				++CurrentUnitIndex;
			}
		}
		
		break;

	default:
		UE_LOG(LogCG, Warning, TEXT("Battle phase not yet implemented"));
	}
}

void ACGGameMode::StartPlacingContainer(AContainer* Container)
{
	if (!CanPlaceContainer.IsBound() || !CanPlaceContainer.Execute(Container))
	{
		GEngine->AddOnScreenDebugMessage(0, 4.f, FColor::Red,
			TEXT("Not enough energy"));
		return;
	}
	
	GamePhase = EGamePhase::PlayerPlacingUnit;
	ContainerBeingPlaced = Container;

	AUnit* SpawnedUnit = GetWorld()->SpawnActor<AUnit>(Container->GetUnit());

	OnStartPlacingContainer.Broadcast(SpawnedUnit);
}

void ACGGameMode::EndPlacingContainer(bool bSuccessful, AUnit* NewUnit)
{
	GamePhase = EGamePhase::PlayerSelectingContainer;

	if (bSuccessful)
	{
		OnEndPlacingContainer.Broadcast(true, ContainerBeingPlaced->GetCost(), NewUnit);
		ContainerBeingPlaced->DestroyContainer();
	}
	else
	{
		OnEndPlacingContainer.Broadcast(false, 0, nullptr);
	}
	ContainerBeingPlaced = nullptr;
}

void ACGGameMode::StartInspection()
{
	GamePhase = EGamePhase::PlayerInspecting;
	OnStartInspection.Broadcast();
}

void ACGGameMode::EndInspection()
{
	GamePhase = EGamePhase::PlayerSelectingContainer;
	OnEndInspection.Broadcast();
}


void ACGGameMode::EndPlayerTurn()
{
	if (TurnNumber % 2 == 1)
	{
		GamePhase = EGamePhase::OpponentTurn;
		OnStartOpponentTurn.Broadcast();
	}
	else
	{
		GamePhase = EGamePhase::Battling;

		BattlePhase = EBattlePhase::Moving;
		bIsCurrentUnitMoving = false;
		bHaveUnitsAttacked = false;
		CurrentUnitIndex = 0;

		OnStartBattling.Broadcast();
	}
}

void ACGGameMode::EndOpponentTurn()
{
	OnEndOpponentTurn.Broadcast();
	if (TurnNumber % 2 == 1)
	{
		GamePhase = EGamePhase::Battling;

		BattlePhase = EBattlePhase::Moving;
		bIsCurrentUnitMoving = false;
		bHaveUnitsAttacked = false;
		CurrentUnitIndex = 0;

		OnStartBattling.Broadcast();
	}
	else
	{
		GamePhase = EGamePhase::PlayerSelectingContainer;
	}
}

void ACGGameMode::SpawnContainer(int32 ForTeamID)
{
	ACGWorldSettings* ContainerWorldSettings =
		UCGLibrary::GetCGWorldSettings(this);
	check(ContainerWorldSettings);

	TArray<TSubclassOf<AContainer>>& InventoryToPickFrom =
		ForTeamID == 0 ? PlayerContainers : OpponentContainers;
	
	if (InventoryToPickFrom.Num() == 0) return;
	
	const TSubclassOf<AContainer> ContainerClassToSpawn = InventoryToPickFrom.Pop();

	
	FContainerSpawnLocationInfoHandle InfoHandle =
		ContainerWorldSettings->FindFirstAvailableLocationForTeamID(ForTeamID);
	SpawnContainerForClassAndInfo(ContainerClassToSpawn, InfoHandle);
}

void ACGGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ACGGameMode::StartBattle);
}

void ACGGameMode::StartBattle()
{
	const ACGWorldSettings* ContainerWorldSettings =
		UCGLibrary::GetCGWorldSettings(this);
	check(ContainerWorldSettings);

	PlayerContainers.Append(
		UCGLibrary::GetCGGameInstance(this)->GetContainerInventory()
		);
	OpponentContainers.Append(
		UCGLibrary::GetCGWorldSettings(this)->GetOpponent()->GetInventory()
		);
	
	UCGLibrary::ShuffleArray(PlayerContainers);
	UCGLibrary::ShuffleArray(OpponentContainers);

	for (int TeamID = 0; TeamID < 2; ++TeamID)
	{
		for (int i = 0; i < ContainerWorldSettings->GetStartingContainerCount(); ++i)
		{
			SpawnContainer(TeamID);
		}
	}
	
}

void ACGGameMode::EndTurn()
{
	++TurnNumber;
	OnEndBattling.Broadcast(TurnNumber);

	if (TurnNumber % 2 == 1)
	{
		GamePhase = EGamePhase::PlayerSelectingContainer;
	}
	else
	{
		GamePhase = EGamePhase::OpponentTurn;
		OnStartOpponentTurn.Broadcast();
	}

	for (int TeamID = 0; TeamID < 2; ++TeamID)
	{
		SpawnContainer(TeamID);
	}
}

void ACGGameMode::SpawnContainerForClassAndInfo(TSubclassOf<AContainer> ContainerClass,
                                                       const FContainerSpawnLocationInfoHandle& InfoHandle) const
{
	if (!InfoHandle.IsValid()) return;

	FContainerSpawnLocationInfo& Info = *InfoHandle.Get();
	UWorld* World = GetWorld();
	check(World);

	AContainer* NewContainer = World->SpawnActor<AContainer>(ContainerClass,
		Info.GetSpawnLocation(), FRotator::ZeroRotator);
	NewContainer->SetTeamID(Info.GetTeamID());
	Info.SetOccupyingContainer(NewContainer);
}
