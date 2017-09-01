#pragma once

#include <windows.h>
#include "compoundtask.h"
#include "..\GameLib\Event.h"

class WaitAndFireEventTask : public CompoundTask
{
public:
	WaitAndFireEventTask( UINT delayInMS, EventType type);
	~WaitAndFireEventTask(){};

	bool update();
};

