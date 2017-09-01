#pragma once

#include <vector>
#include "..\gamelib\trackable.h"

class BaseTask;

class TaskCollection :	public Trackable
{
public:
	TaskCollection( bool topOnly = true );
	~TaskCollection();

	bool update();//calls update on appropriate Tasks and returns true if complete

	void addTask( BaseTask* pTask );
	//only implement if needed
	//void removeTask( BaseTask* pTask );

	inline bool isEmpty() const { return mTasks->empty(); };

private:
	bool mProcessTopOnly;
	std::vector<BaseTask*>* mTasks;//will be dynamically created

	void removeCompletedTasks();
};

