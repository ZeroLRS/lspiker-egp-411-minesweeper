#include "CellRevealedNotification.h"

CellRevealedNotification::CellRevealedNotification(unsigned int cell) : Event(CELL_REVEALED_NOTIFICATION)
{
	mCell = cell;
}

CellRevealedNotification::~CellRevealedNotification()
{
}
