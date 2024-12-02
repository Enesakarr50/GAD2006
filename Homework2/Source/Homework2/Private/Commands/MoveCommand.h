#pragma once

#include "CoreMinimal.h"
#include "Command.h"





class MoveCommand : public Command
{


public:
	
	MoveCommand(FSGridPosition Src, FSGridPosition Dest);
	
	virtual ~MoveCommand();

	
	virtual void Execute() override;
	virtual void Undo() override;

public:
	
	FSGridPosition Source, Destination;
};
