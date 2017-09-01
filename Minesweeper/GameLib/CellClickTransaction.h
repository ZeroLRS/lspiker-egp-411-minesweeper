#pragma once
#include "transaction.h"
#include <windows.h>

class CellClickTransaction : public Transaction
{
public:
	CellClickTransaction( UINT theCellIndex );
	virtual ~CellClickTransaction();

	inline UINT getCellIndex() const { return mCellIndexClicked; };

private:
	UINT mCellIndexClicked;
};

