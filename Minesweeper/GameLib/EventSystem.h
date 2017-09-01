#pragma once

#include <map>
#include <assert.h>
#include "Trackable.h"

class BaseEvent;
class Event;
class TransactionHandler;
enum EventType;

typedef void (__cdecl *EventCallbackFunction)( const Event&, TransactionHandler* );


class EventSystem:public Trackable
{
public:
	void fireEvent( const Event& theEvent );
	void registerCallback( EventType type, EventCallbackFunction callbackFunction );
	void unregisterCallback( EventType type, EventCallbackFunction callbackFunction );

	static inline EventSystem* getEventSystem() { return mspInstance; };
	static inline void createEventSystem(){ assert( mspInstance == NULL ); mspInstance = new EventSystem; };
	static inline void destroyEventSystem(){ delete mspInstance; mspInstance = NULL; };
private:
	static EventSystem* mspInstance;

	std::multimap< EventType, EventCallbackFunction > mCallbacks;

	void dispatchAllEvents( const Event& theEvent );
	EventSystem();
	~EventSystem();

};

