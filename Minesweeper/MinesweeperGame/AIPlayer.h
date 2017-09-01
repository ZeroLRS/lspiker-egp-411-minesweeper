#pragma once

#include "../GameLib/Player.h"

class AIHolder;

class AIPlayer : public Player
{
public:
	AIPlayer( AIHolder* pHolder );
	virtual ~AIPlayer();

	void update(){};

	void setActive( bool isActive );

private:
	AIHolder* mpHolder;
};

