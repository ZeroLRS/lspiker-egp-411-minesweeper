#pragma once

#include <string>
#include <map>
#include <windows.h>
#include "..\GameLib\Trackable.h"
#include "..\GameLib\EventSystem.h"

class GameState;
class TransactionHandler;

typedef std::string FuncID;

const FuncID MAKE_DECISION = "makeDecision";

class AIHolder: public Trackable
{
public:
	AIHolder( const std::string& playerName, const std::string& dllPath);
	~AIHolder();

	inline const std::string& getName() const { return mName; };

	void setActive( bool flag );

private:
	std::string mName;
	HINSTANCE mDllHandle;
	EventCallbackFunction mFunction;
	bool mActive;

};