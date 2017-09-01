#pragma once
#include "event.h"

class GameView;

class PickCellEvent : public Event
{
public:
	PickCellEvent( const GameView& gameView );
	~PickCellEvent(){};

	inline const GameView& getGameView() const { return mGameView; };

private:
	const GameView& mGameView; 
};

