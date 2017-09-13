#include "GameResetNotification.h"

GameResetNotification::GameResetNotification(size_t width, size_t height, unsigned int mines) : Event(GAME_RESET_NOTIFICATION)
{
	mWidth = width;
	mHeight = height;
	mMines = mines;
}

GameResetNotification::~GameResetNotification()
{
}
