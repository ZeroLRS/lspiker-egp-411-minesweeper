#pragma once

#include "basetask.h"
#include "..\GameLib\Event.h"

class FireEventTask : public BaseTask
{
public:
	FireEventTask( EventType type );
	~FireEventTask(){};

	bool update();

private:
	EventType mEventType;
};

