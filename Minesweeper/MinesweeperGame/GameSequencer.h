#pragma once

#include "..\gamelib\trackable.h"
#include <windows.h>

class GameSequencer : public Trackable
{
public:
	enum GameType
	{
		LARGE,
		MEDIUM,
		SMALL,
		END_SIMULATION
	};

	GameSequencer();
	~GameSequencer(){};

	void init();
	bool setNextGameParams();//returns false when simulation should end
	inline GameType getCurrentGameType() const { return mCurrentGameType; };

private:

	UINT mNumLargeStillToPlay;
	UINT mNumMediumStillToPlay;
	UINT mNumSmallStillToPlay;
	GameType mCurrentGameType;
};

