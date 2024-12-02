#include "MoveCommand.h"
#include "GameGrid.h"
#include "UnitBase.h"
#include "GameSlot.h"



MoveCommand::MoveCommand(FSGridPosition Src, FSGridPosition Dest):
Source(Src), Destination(Dest)
{
}


MoveCommand::~MoveCommand()
{
}


void MoveCommand::Execute()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Executing Move..."));

	
	AGameSlot* SlotA = AGameGrid::FindSlot(Source);
	AGameSlot* SlotB = AGameGrid::FindSlot(Destination);

	if (SlotA && SlotB)
	{
		AUnitBase* UnitA = SlotA->Unit;
		if (UnitA)
		{
			
			UnitA->AssignToSlot(SlotB);
			SlotB->SetState(Highlighted); 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No unit found in Source Slot"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid source or destination slot"));
	}
}

void MoveCommand::Undo()
{
	UE_LOG(LogTemp, Warning, TEXT("Reverting Move..."));

	// Find the slots using the source and destination positions
	AGameSlot* SlotA = AGameGrid::FindSlot(Source);
	AGameSlot* SlotB = AGameGrid::FindSlot(Destination);

	if (SlotA && SlotB)
	{
		AUnitBase* UnitB = SlotB->Unit;
		if (UnitB)
		{
			SlotB->SetState(Default);
			UnitB->AssignToSlot(SlotA);
			SlotA->SetState(Highlighted);  
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No unit found in Destination Slot"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid source or destination slot"));
	}
}





