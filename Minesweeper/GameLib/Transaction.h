#pragma once

#include <windows.h>
#include "Trackable.h"

enum TransactionType
{
	INVALID_TRANSACTION_TYPE = -1,
	INIT_TRANSACTION,
	CELL_CLICK_TRANSACTION
};

class Transaction: public Trackable
{
public:
	Transaction();
	Transaction( TransactionType type );
	virtual ~Transaction();

	TransactionType getType() const { return mType; };

private:
	TransactionType mType;
	int mID;

	static int msNextID;
};

