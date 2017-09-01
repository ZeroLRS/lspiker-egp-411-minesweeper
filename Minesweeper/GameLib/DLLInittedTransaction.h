#include "Transaction.h"
#include <string>

class DLLInittedTransaction :public Transaction
{
public:
	DLLInittedTransaction(const std::string& message) : Transaction(INIT_TRANSACTION), mMessage(message){};
	virtual ~DLLInittedTransaction(){};

	const std::string& getMessage() const { return mMessage; };
private:
	std::string mMessage;
};