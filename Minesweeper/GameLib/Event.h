#pragma once

#include <string>
#include "../GameLib/BaseEvent.h"

using namespace std;

enum EventType
{
	PICK_A_CELL_EVENT = BaseEventType::INVALID_EVENT_TYPE + 1,
	MINE_HIT_EVENT,
	CELL_REVEALED_EVENT,
	GAME_WON_EVENT,
	GAME_QUIT_EVENT,
	MOUSE_CLICKED_EVENT,
	GAME_RESET_EVENT,
	INIT_EVENT,
	CLEANUP_EVENT,
	NUM_EVENT_TYPES
};

const string EVENT_NAMES[NUM_EVENT_TYPES] =	{	"Reveal a Cell Event",
												"Mine Hit Event",
												"Cell Revealed Event",
												"Game Won Event",
												"Game Quit Event"
											};

class Event:public BaseEvent
{
public:
	Event( EventType type );
	virtual ~Event();

	inline EventType getType() const { return (EventType)getEventType(); };
	const string& getEventName() const;

private:
};