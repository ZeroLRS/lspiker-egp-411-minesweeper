#pragma once
#include "event.h"
#include <windows.h>


class ResetGameEvent : public Event
{
public:
	ResetGameEvent(UINT numMines, UINT width, UINT height);
	~ResetGameEvent(){};

	inline UINT getNumMines() const { return mNumMines; };
	inline UINT getWidth() const { return mWidth; };
	inline UINT getHeight() const { return mHeight; };

private:
	UINT mNumMines;
	UINT mWidth;
	UINT mHeight;
};

