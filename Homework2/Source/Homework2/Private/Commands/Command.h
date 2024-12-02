#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"




class Command
{

public:
	Command() {};
	virtual ~Command() {};

	// Virtual functions for command behavior
	virtual void Execute() = 0;
	virtual void Undo() = 0;
	virtual void Update(float DeltaTime) {};
	virtual bool IsExecuting() { return false; };
};
