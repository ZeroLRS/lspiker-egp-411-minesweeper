#pragma once

#include <string>
#include "..\gamelib\player.h"

class TransactionHandler;

class HumanPlayer:public Player
{
public:
	HumanPlayer(const std::string& name, TransactionHandler* pHandler);
	virtual ~HumanPlayer();

	virtual void update();
	inline void setActive( bool isActive ) { mActive = isActive; };

private:
	TransactionHandler* mpHandler;
	bool mActive;

};

