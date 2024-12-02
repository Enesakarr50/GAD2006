// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "UnitBase.h"
#include "GameGrid.h"
#include "TBPlayerController.h"
#include "Commands/Command.h"
#include "Commands/MoveCommand.h"
#include "Engine/World.h"

class ATBPlayerController;
// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameManager::OnActorClicked(AActor* Actor, FKey Button)
{
	UE_LOG(LogTemp, Warning, TEXT("Triggered."));
	if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentCommand is executing or valid, skipping."));
		return;
	}

	AGameSlot* Slot = Cast<AGameSlot>(Actor);
	if (!Slot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor is not a GameSlot."));
		return;
	}

	if (!PlayerUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerUnit is null."));
		return;
	}

	if (Slot->Unit == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot's Unit is null, proceeding to execute MoveCommand."));
		TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(PlayerUnit->Slot->GridPosition, Slot->GridPosition);
		
		CommandPool.Add(Cmd);
		Cmd->Execute();
		CurrentCommand = Cmd;
		UE_LOG(LogTemp, Warning, TEXT("MoveCommand executed and set as CurrentCommand."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot's Unit is occupied."));
	}
}


void AGameManager::CreateLevelActors(FSLevelInfo& LevelInfo)
{
	UE_LOG(LogTemp, Display, TEXT("Creating LevelActors"));
	PlayerUnit = nullptr;

	if (!GameGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("GameGrid is null."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Starting to create level actors for level %d"), CurrentLevel);
	for (auto UnitInfo : LevelInfo.Units)
	{
		if (AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Retrieved slot at (%d, %d)."), UnitInfo.StartPosition.Col, UnitInfo.StartPosition.Row);
			Slot->SpawnUnitHere(UnitInfo.UnitClass);

			if (Slot->Unit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Unit spawned successfully."));
				if (Slot->Unit->IsControlledByThePlayer())
				{
					PlayerUnit = Slot->Unit;
					UE_LOG(LogTemp, Warning, TEXT("Player-controlled unit assigned."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Unit spawning failed for slot (%d, %d)."), UnitInfo.StartPosition.Col, UnitInfo.StartPosition.Row);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve slot at position: (%d, %d)."), UnitInfo.StartPosition.Col, UnitInfo.StartPosition.Row);
		}
	}

	if (!PlayerUnit)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerUnit was not assigned."));
	}
}


// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerController = GetWorld()->GetFirstPlayerController<ATBPlayerController>();
	if (PlayerController)
	{
		PlayerController->GameManager = this;	
	}
	
	if(Levels.IsValidIndex(CurrentLevel))
	{
		CreateLevelActors(Levels[CurrentLevel]);
	}
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCommand.IsValid())
	{
		CurrentCommand->Update(DeltaTime);
	}
		
}

bool AGameManager::UndoLastMove()
{
	// Check if the command pool has any commands
	if (CommandPool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No previous move to undo."));
		return false;  // No move to undo
	}

	// Get the last command from the pool
	TSharedRef<MoveCommand> LastMoveCommand = StaticCastSharedRef<MoveCommand>(CommandPool.Last());

	// Perform the undo on the last command
	LastMoveCommand->Undo(); // This will move the unit back to its previous position

	// Remove the last command from the pool
	CommandPool.RemoveAt(CommandPool.Num() - 1);

	UE_LOG(LogTemp, Warning, TEXT("Move undone successfully."));
	return true;
}










