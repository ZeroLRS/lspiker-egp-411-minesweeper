#pragma once

#include <windows.h>
#include "..\gamelib\trackable.h"

typedef UINT TaskID;

class BaseTask : public Trackable
{
public:
	BaseTask();
	virtual ~BaseTask(){};

	virtual bool update() = 0;//return true if complete

	inline TaskID getID() const { return mID; };
	inline bool isDone() const { return mDone; };

private:
	bool mDone;
	const TaskID mID;
	static TaskID msNextTaskID;

protected:
	inline void flagAsDone() { mDone = true; };
};

