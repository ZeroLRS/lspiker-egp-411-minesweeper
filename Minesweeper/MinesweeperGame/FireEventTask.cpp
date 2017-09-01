#include "FireEventTask.h"
#include "Game.h"
#include "..\GameLib\EventSystem.h"
#include "..\GameLib\PickCellEvent.h"
#include "..\GameLib\ResetGameEvent.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\GameState.h"


FireEventTask::FireEventTask(EventType type)
	:mEventType( type )
{
}

bool FireEventTask::update()
{
	switch( mEventType )
	{
		case PICK_A_CELL_EVENT:
		{
			EventSystem::getEventSystem()->fireEvent(PickCellEvent(GameView(gpGame->getGameState())));
			flagAsDone();
			return true;
		}
		case GAME_RESET_EVENT:
		{
			Grid* pGrid = gpGame->getGameState()->getGrid();
			EventSystem::getEventSystem()->fireEvent(ResetGameEvent(pGrid->getNumMines(), pGrid->getWidth(), pGrid->getHeight()));
			flagAsDone();
			return true;
		}
		default:
		{
			cout << "fire\n";
			EventSystem::getEventSystem()->fireEvent(Event(mEventType));
			flagAsDone();
			return true;
		}
	}
	return false;
}

