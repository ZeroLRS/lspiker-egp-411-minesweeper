#include "GameView.h"



GameView::GameView(GameState* pState)
	:mpState( pState )
{

}

GameView::~GameView()
{
}

size_t GameView::getNumAdjacentMines(UINT index) const
{
	Grid* pGrid = mpState->getGrid();
	Cell* pCell = pGrid->getCellAtIndex(index);
	if (pCell->isRevealed())
	{
		return pGrid->getNumAdjacentMines(index);
	}
	else
	{
		return UINT_MAX;
	}
}
