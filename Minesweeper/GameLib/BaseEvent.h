#pragma once
#include "trackable.h"

enum BaseEventType
{
	INVALID_EVENT_TYPE = -1
};

class BaseEvent : public Trackable
{
protected:
	BaseEvent( BaseEventType type );//only subclasses may contruct one
	inline BaseEventType getEventType() const { return mType; };

	BaseEventType mType;
};

