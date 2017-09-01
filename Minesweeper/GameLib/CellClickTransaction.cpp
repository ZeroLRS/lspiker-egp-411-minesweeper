#include "CellClickTransaction.h"


CellClickTransaction::CellClickTransaction( UINT theCellIndex )
	:Transaction(CELL_CLICK_TRANSACTION)
	,mCellIndexClicked(theCellIndex)
{
}

CellClickTransaction::~CellClickTransaction()
{
}
