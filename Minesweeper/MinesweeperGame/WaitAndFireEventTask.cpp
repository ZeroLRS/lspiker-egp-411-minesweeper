#include "WaitAndFireEventTask.h"
#include "WaitTask.h"
#include "FireEventTask.h"



WaitAndFireEventTask::WaitAndFireEventTask(UINT delayInMS, EventType type)
	:CompoundTask(true)
{
	WaitTask* pWait = new WaitTask( delayInMS );
	addTask( pWait );

	FireEventTask* pFire = new FireEventTask( type );
	addTask( pFire );
}

bool WaitAndFireEventTask::update()
{
	bool done =	mTasks.update();
	if( done )
		flagAsDone();
	return done;
}


