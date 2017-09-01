#include "Event.h"

Event::Event(EventType type)
:BaseEvent( (BaseEventType)type )
{
}

Event::~Event()
{
}

const string& Event::getEventName() const
{
	if( mType > 0 && mType < NUM_EVENT_TYPES )
	{
		return EVENT_NAMES[mType];
	}
	else
	{
		static const string nullString;
		return nullString;
	}
}
