#include "ResetGameEvent.h"

ResetGameEvent::ResetGameEvent(UINT numMines, UINT width, UINT height)
	:Event(GAME_RESET_EVENT)
	,mNumMines(numMines)
	,mWidth(width)
	,mHeight(height)
{

}

