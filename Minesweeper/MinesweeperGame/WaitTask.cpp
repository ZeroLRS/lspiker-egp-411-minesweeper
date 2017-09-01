#include <iostream>
#include "WaitTask.h"

using namespace std;

WaitTask::WaitTask(UINT delayInMS)
	:mTimeToElapse( delayInMS )
	,mStarted( false )
{
}

bool WaitTask::update()
{
	if( !mStarted )
	{
		mTimer.start();
		mStarted = true;
	}
	if( mTimer.getElapsedTime() >= mTimeToElapse )
	{
		flagAsDone();
	}

	return isDone();
}
