#pragma once
#include "Event.h"
class GameResetNotification : public Event
{
public:
	GameResetNotification(size_t width, size_t height, unsigned int mines);
	~GameResetNotification();

	size_t getWidth() const { return mWidth; }
	size_t getHeight() const { return mHeight; }
	unsigned int getNumMines() const { return mMines; }

private:
	size_t mWidth;
	size_t mHeight;
	unsigned int mMines;
};

