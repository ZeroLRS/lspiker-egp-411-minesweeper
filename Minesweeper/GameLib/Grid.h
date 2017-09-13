#pragma once

#include "Trackable.h"
#include <Windows.h>
#include <vector>

class Cell;

class Grid: public Trackable
{
public:
	Grid( size_t width, size_t height );
	Grid( const Grid& rhs );
	~Grid();

	Cell* getCellAt( UINT x, UINT y ) const;
	Cell* getCellAtIndex( UINT index ) const;
	void placeRandomMines( size_t numMines );
	std::vector<UINT> getAdjacentCellIndices( UINT index ) const;
	size_t getNumAdjacentMines( UINT index ) const;

	inline UINT getIndexFromXY( UINT x, UINT y ) const { return ( y * mWidth) + x; };
	void getXYFromIndex( UINT index, UINT& x, UINT& y ) const;
	bool isInBounds( UINT x, UINT y ) const;

	inline size_t getNumCells() const { return mWidth * mHeight; };
	inline size_t getWidth() const { return mWidth; }
	inline size_t getHeight() const { return mHeight; }

	size_t getNumHiddenNonMines() const;
	size_t getNumMines() const;

	bool hasHiddenNonMinesLeft() const;
	void revealAll();
	void revealZeroMinesAdjacentCells( UINT index );
	bool hasRevealedCellAdjacent( UINT index ) const;

	void clearAllCells();

	void setLastCellIndex(UINT i) { mLastCellIndex = i; }
	UINT getLastCellIndex() const { return mLastCellIndex; }
private:
	Cell* mpCells; //dynamically allocated array of cell's
	size_t mWidth;
	size_t mHeight;
	UINT mLastCellIndex;

	//invalidate functions
	void operator=(const Grid&);
};

