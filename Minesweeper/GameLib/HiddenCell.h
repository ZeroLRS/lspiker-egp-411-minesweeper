#pragma once

#include "Trackable.h"
#include "Cell.h"

class HiddenCell: public Trackable
{
public:
	HiddenCell(Cell* pCell);
	~HiddenCell(){};

	inline bool hasMine() const { return mpCell->hasMine() && mpCell->isRevealed(); };
	inline bool isRevealed() const { return mpCell->isRevealed(); };

private:
	Cell* mpCell;
};

