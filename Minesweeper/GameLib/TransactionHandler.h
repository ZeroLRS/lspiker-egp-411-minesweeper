#pragma once

#include <queue>
#include <mutex>
#include "Trackable.h"

class Transaction;
class GameState;

typedef void (__cdecl *TransactionHandlerFunction)(Transaction*, GameState*);

class TransactionHandler: public Trackable
{
public:
	static void createTransactionHandler( TransactionHandlerFunction handler, GameState* pGameState );
	static void destroyTransactionHandler();
	void postTransaction( Transaction* pTransaction );
	void processAllTransactions();
	inline static TransactionHandler* getHandler() { return mspInstance; };

private:
	std::queue<Transaction*> mTransactions;
	TransactionHandlerFunction mHandlerFunction;
	GameState* mpGameState;
	std::unique_lock<std::mutex> mLock;

	static TransactionHandler* mspInstance;

	TransactionHandler( TransactionHandlerFunction handler, GameState* pGameState );
	~TransactionHandler();

	Transaction* popTransaction();
};




