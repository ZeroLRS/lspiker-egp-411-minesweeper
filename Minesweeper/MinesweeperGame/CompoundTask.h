#pragma once

#include "basetask.h"
#include "TaskCollection.h"

class CompoundTask : public BaseTask
{
public:
	CompoundTask( bool topOnly );
	virtual ~CompoundTask(){};

	void addTask( BaseTask* pTask );

protected:
	TaskCollection mTasks;
};

