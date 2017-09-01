#pragma once

#include <windows.h>
#include "basetask.h"
#include "..\GameLib\Timer.h"

class WaitTask :
	public BaseTask
{
public:
	WaitTask( UINT delayInMS );
	virtual ~WaitTask(){};

	bool update();

private:
	Timer mTimer;
	UINT mTimeToElapse;
	bool mStarted;
};

