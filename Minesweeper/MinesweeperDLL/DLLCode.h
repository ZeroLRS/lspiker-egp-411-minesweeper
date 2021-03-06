#pragma once

#include <string>

class Event;
class TransactionHandler;

extern "C"
{
	__declspec(dllexport) void makeDecision(const Event& theEvent, TransactionHandler* pHandler);
}

void init(TransactionHandler* pHandler);
std::string getMessageFromStringTable();
std::string getMessageFromFileResource();
