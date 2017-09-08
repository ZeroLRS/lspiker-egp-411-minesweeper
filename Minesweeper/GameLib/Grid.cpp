#include "Grid.h"
#include "Cell.h"
#include <assert.h>
#include <vector>
#include <algorithm>
#include <set>
#include "EventSystem.h"
#include "CellRevealedNotification.h"

using namespace std;


Grid::Grid(size_t width, size_t height)
	:mpCells(NULL)
	,mWidth(width)
	,mHeight(height)
	
{
	size_t size = width * height;
	mpCells = new Cell[ size ];
}

Grid::Grid(const Grid& rhs)
	:mpCells(NULL)
	,mWidth(rhs.mWidth)
	,mHeight(rhs.mHeight)
{
	size_t size = mWidth * mHeight;
	mpCells = new Cell[ size ];

	for( UINT i=0; i<size; i++ )
	{
		mpCells[i] = rhs.mpCells[i];
	}
}

Grid::~Grid()
{
	delete [] mpCells;
}

Cell* Grid::getCellAt(UINT x, UINT y) const
{
	return getCellAtIndex( getIndexFromXY( x, y ) );
}

Cell* Grid::getCellAtIndex(UINT index) const
{
	assert(index < (mWidth * mHeight) );
	return &(mpCells[index]); 
}

const UINT NUM_DIRECTIONS = 8;
const int YMODS[NUM_DIRECTIONS] = {	-1,	-1,	-1, 
									 0,	 	 0,
									 1,	 1,	 1 };

const int XMODS[NUM_DIRECTIONS] = {	-1,	0,	1, 
									-1,		1,
									-1,	0,	1 };


std::vector<UINT> Grid::getAdjacentCellIndices( UINT index ) const
{
	UINT x, y;
	getXYFromIndex( index, x, y );
	vector<UINT> indices;

	for( UINT i=0; i<NUM_DIRECTIONS; i++ )
	{
		UINT newX = XMODS[i] + x;
		UINT newY = YMODS[i] + y;
		if( isInBounds( newX, newY ) )
		{
			indices.push_back( getIndexFromXY( newX, newY ) );
		}
	}
	return indices;
}

void Grid::placeRandomMines( size_t numMines )
{
	//create vector of indices which will be shuffled
	size_t numCells = mWidth * mHeight;
	vector<int> indices( numCells );

	for( size_t i=0; i<numCells; i++ )
	{
		indices[i] = i;
	}

	random_shuffle( indices.begin(), indices.end() );

	//place the first numMines mines
	for( size_t i=0; i<numMines; i++ )
	{
		mpCells[indices[i]].setFlags( MINE_CELL );
	}

}

void Grid::getXYFromIndex(UINT index, UINT& x, UINT& y) const
{
	x = index % mWidth;
	y = index / mWidth;
}

bool Grid::isInBounds(UINT x, UINT y) const
{
	if( x >= mWidth || y>= mHeight )
		return false;
	else
		return true;
}

size_t Grid::getNumAdjacentMines( UINT index ) const
{
	vector<UINT> adjacentIndices = getAdjacentCellIndices( index );
	size_t count = 0;
	for( UINT i=0; i<adjacentIndices.size(); i++ )
	{
		if( getCellAtIndex(adjacentIndices[i])->hasMine() )
		{
			count++;
		}
	}
	return count;
}

bool Grid::hasHiddenNonMinesLeft() const
{
	bool retVal = false;
	size_t numCells = getNumCells();

	for( UINT i=0; i<numCells; i++ )
	{
		Cell* pCell = getCellAtIndex( i );
		if( !pCell->hasMine() )//not a mine
		{
			if( !pCell->isRevealed() )
			{
				retVal = true;
				break;
			}
		}
	}
	return retVal;
}

void Grid::revealAll()
{
	size_t numCells = getNumCells();

	for( UINT i=0; i<numCells; i++ )
	{
		Cell* pCell = getCellAtIndex( i );
		pCell->setFlags(REVEALED_CELL);
	}
}

void Grid::revealZeroMinesAdjacentCells(UINT index)
{
	set<UINT> toProcess;
	set<UINT> alreadyProcessed;
	if( getNumAdjacentMines( index ) == 0 )
	{
		toProcess.insert(index);
	}
	getCellAtIndex( index )->setFlags(REVEALED_CELL);
	EventSystem::getEventSystem()->fireEvent(CellRevealedNotification(index));

	while( !toProcess.empty() )
	{
		//get current index and remove it from set of indices to process
		UINT current = *( toProcess.begin() );
		toProcess.erase( toProcess.begin() );
		alreadyProcessed.insert( current );

		//get all adjacent indices
		vector<UINT> adjIndices = getAdjacentCellIndices( current );
		size_t adjSize = adjIndices.size();
		for( UINT i=0; i<adjSize; i++ )
		{
			//reveal cell
			UINT adjIndex = adjIndices[i];
			if (!getCellAtIndex(adjIndex)->isRevealed()) EventSystem::getEventSystem()->fireEvent(CellRevealedNotification(adjIndex));
			getCellAtIndex( adjIndex )->setFlags(REVEALED_CELL);

			if( alreadyProcessed.find( adjIndex ) == alreadyProcessed.end() )//hasn't already been processed
			{
				UINT numMinesAdj = getNumAdjacentMines( adjIndex );
				if( numMinesAdj == 0 )
				{
					toProcess.insert( adjIndex );
				}
			}
		}
	}

}

bool Grid::hasRevealedCellAdjacent( UINT index ) const
{
	vector<UINT> adjIndices = getAdjacentCellIndices( index );
	size_t adjSize = adjIndices.size();
	for( UINT i=0; i<adjSize; i++ )
	{
		if(	getCellAtIndex( adjIndices[i] )->isRevealed() )
		{
			return true;
		}
	}
	return false;
}

void Grid::clearAllCells()
{
	size_t numCells = getNumCells();

	for( UINT i=0; i<numCells; i++ )
	{
		Cell* pCell = getCellAtIndex( i );
		pCell->setFlags( 0 , true );
	}
}

size_t Grid::getNumHiddenNonMines() const
{
	size_t numCells = getNumCells();
	size_t count = 0;

	for (UINT i = 0; i < numCells; i++)
	{
		Cell* pCell = getCellAtIndex(i);
		if (!pCell->hasMine() && !pCell->isRevealed())
		{
			count++;
		}
	}
	return count;
}

size_t Grid::getNumMines() const
{
	size_t numCells = getNumCells();
	size_t count = 0;

	for (UINT i = 0; i < numCells; i++)
	{
		Cell* pCell = getCellAtIndex(i);
		if (pCell->hasMine())
		{
			count++;
		}
	}
	return count;
}
