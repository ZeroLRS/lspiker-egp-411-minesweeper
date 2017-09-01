#include "Cell.h"
#include <assert.h>

Cell::Cell(bool mine, bool revealed, bool flagged)
	:mFlags(0x0)
{
	if( mine )
		mFlags |= MINE_CELL;
	if( revealed )
		mFlags |= REVEALED_CELL;
	if( flagged )
		mFlags |= FLAGGED_CELL;
}

Cell::Cell()
	:mFlags(EMPTY_CELL)
{
}

Cell::~Cell()
{
}

void Cell::setFlags(byte newFlags, bool overwrite /*= false */)
{
	assert( newFlags <= ( MINE_CELL | REVEALED_CELL | FLAGGED_CELL ) );

	if( overwrite )
	{
		mFlags = newFlags;
	}
	else
	{
		mFlags |= newFlags;
	}
}

void Cell::clearFlags(byte newFlags)
{
	if ((newFlags & MINE_CELL) > 0)
	{
		mFlags &= ~(MINE_CELL);
	}
	if ((newFlags & REVEALED_CELL) > 0)
	{
		mFlags &= ~(REVEALED_CELL);
	}
	if ((newFlags & FLAGGED_CELL) > 0)
	{
		mFlags &= ~(FLAGGED_CELL);
	}
}
