#include "EventSystem.h"
#include "Event.h"
#include "TransactionHandler.h"

EventSystem* EventSystem::mspInstance = NULL;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::registerCallback(EventType type, EventCallbackFunction callbackFunction)
{
	mCallbacks.insert( pair< EventType, EventCallbackFunction >(type, callbackFunction) );
}

void EventSystem::unregisterCallback(EventType type, EventCallbackFunction callbackFunction)
{
	pair<multimap<EventType,EventCallbackFunction>::iterator,multimap<EventType,EventCallbackFunction>::iterator> ret;

	ret = mCallbacks.equal_range( type );
	multimap<EventType,EventCallbackFunction>::iterator iter;

	for( iter = ret.first; iter != ret.second; ++iter )
	{
		if( iter->second == callbackFunction )
		{
			mCallbacks.erase( iter );
			break;//to prevent using invalidated iterator
		}
	}
}

void EventSystem::fireEvent( const Event& theEvent )
{
	dispatchAllEvents( static_cast<const Event&>(theEvent) );
}

void EventSystem::dispatchAllEvents( const Event& theEvent )
{
	TransactionHandler* pHandler = TransactionHandler::getHandler();

	pair<multimap<EventType,EventCallbackFunction>::iterator,multimap<EventType,EventCallbackFunction>::iterator> ret;
	ret = mCallbacks.equal_range( theEvent.getType() );

	multimap<EventType,EventCallbackFunction>::iterator iter;
	for( iter = ret.first; iter != ret.second; ++iter )
	{
		EventCallbackFunction theFunction = iter->second;
		theFunction( theEvent, pHandler );//call the callback function!
	}

}