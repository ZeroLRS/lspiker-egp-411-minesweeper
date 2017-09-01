//#include "Game.h"
#include "TransactionHandler.h"
#include "Transaction.h"
#include <assert.h>

using namespace std;

TransactionHandler* TransactionHandler::mspInstance = NULL;

TransactionHandler::TransactionHandler( TransactionHandlerFunction handler, GameState* pGameState )
	:mHandlerFunction(handler)
	,mpGameState(pGameState)
{
}

TransactionHandler::~TransactionHandler()
{
	//mLock.lock();

	//mLock.unlock();
}

void TransactionHandler::createTransactionHandler( TransactionHandlerFunction handler, GameState* pGameState )
{
	assert( mspInstance == NULL );
	mspInstance = new TransactionHandler( handler, pGameState );
}

void TransactionHandler::destroyTransactionHandler()
{
	assert( mspInstance != NULL );
	delete mspInstance;
	mspInstance = NULL;
}

void TransactionHandler::postTransaction( Transaction* pTransaction )
{
	//mLock.lock();

	mTransactions.push( pTransaction );

	//mLock.unlock();
}

Transaction* TransactionHandler::popTransaction()
{
	//mLock.lock();

	if( mTransactions.empty() )
	{
		return NULL;
	}
	else
	{
		Transaction* pFront = mTransactions.front();
		mTransactions.pop();
		return pFront;
	}

	//mLock.unlock();

}

void TransactionHandler::processAllTransactions()
{
	while( !mTransactions.empty() )
	{
		Transaction* pTransaction = popTransaction();
		assert( pTransaction != NULL );
		mHandlerFunction( pTransaction, mpGameState );
	}
}
