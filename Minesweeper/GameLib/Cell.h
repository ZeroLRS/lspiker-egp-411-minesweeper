#pragma once

#include "Trackable.h"
#include <windows.h>


const byte EMPTY_CELL = 0x0;
const byte MINE_CELL = 0x1;
const byte REVEALED_CELL = 0x2;
const byte FLAGGED_CELL = 0x4;


class Cell: public Trackable
{
public:
	Cell( bool mine, bool revealed, bool flagged );
	Cell();
	~Cell();

	inline bool hasMine() const { return ( (mFlags & MINE_CELL) > 0 ); };
	inline bool isRevealed() const { return ( (mFlags & REVEALED_CELL) > 0 ); };
	inline bool isFlagged() const { return ( (mFlags & FLAGGED_CELL) > 0 ); };

	void setFlags(byte newFlags, bool overwrite = false);
	void clearFlags(byte newFlags);


private:
	byte mFlags;
};

