#include "Transaction.h"

int Transaction::msNextID = 0;

Transaction::Transaction()
	:mType(INVALID_TRANSACTION_TYPE)
	,mID(msNextID++)
{

}

Transaction::Transaction( TransactionType type )
	:mType(type)
	,mID(msNextID++)
{

}

Transaction::~Transaction()
{
}

