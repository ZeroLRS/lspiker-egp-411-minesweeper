#include "PickCellEvent.h"

PickCellEvent::PickCellEvent(const GameView& gameView)
	:Event(PICK_A_CELL_EVENT)
	,mGameView(gameView)
{
}
