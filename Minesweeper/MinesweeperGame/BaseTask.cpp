#include "BaseTask.h"

TaskID BaseTask::msNextTaskID = 0;

BaseTask::BaseTask()
	:mID( msNextTaskID++ )
	,mDone(false)
{
}
