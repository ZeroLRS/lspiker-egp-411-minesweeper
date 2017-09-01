#include "CompoundTask.h"


CompoundTask::CompoundTask(bool topOnly)
	:mTasks(topOnly)
{
}

void CompoundTask::addTask(BaseTask* pTask)
{
	mTasks.addTask( pTask );
}
