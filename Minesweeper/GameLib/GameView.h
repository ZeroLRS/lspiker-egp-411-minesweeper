#pragma once
#include "trackable.h"
#include "GameState.h"
#include "HiddenCell.h"

class GameState;
class Cell;

class GameView : public Trackable
{
public:
	GameView( GameState* pState );
	~GameView();

	inline size_t getWidth() const { return mpState->getGrid()->getWidth(); };
	inline size_t getHeight() const { return mpState->getGrid()->getHeight(); };
	inline size_t getNumCells() const { return mpState->getGrid()->getNumCells(); };
	inline UINT getIndexFromXY( UINT x, UINT y ) const { return mpState->getGrid()->getIndexFromXY( x, y ); };
	inline void getXYFromIndex( UINT index, UINT& x, UINT& y ) const { return mpState->getGrid()->getXYFromIndex( index, x, y); };
	inline std::vector<UINT> getAdjacentCellIndices( UINT index ) const { return mpState->getGrid()->getAdjacentCellIndices( index ); };
	size_t getNumAdjacentMines( UINT index ) const;
	size_t getTotalNumMines() const { return mpState->getGrid()->getNumMines(); };

	inline HiddenCell getCell( UINT index ) const { return HiddenCell( mpState->getGrid()->getCellAtIndex( index) ); };
	inline HiddenCell getCell( UINT x, UINT y ) const { return HiddenCell( mpState->getGrid()->getCellAt( x, y ) ); };
private:
	GameState* mpState;
};

