#pragma once
#include "Event.h"
class CellRevealedNotification : public Event
{
public:
	explicit CellRevealedNotification(unsigned int cell);
	~CellRevealedNotification();

	unsigned int getRevealedCell() const { return mCell; }

private:
	unsigned int mCell;
};

