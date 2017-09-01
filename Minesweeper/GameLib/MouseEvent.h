#pragma once
#include "event.h"
#include <windows.h>

enum MouseButton
{
	LEFT_MOUSE_BUTTON = 1,
	RIGHT_MOUSE_BUTTON = 2,
	MIDDLE_MOUSE_BUTTON = 3
};

class MouseEvent : public Event
{
public:
	MouseEvent( UINT x, UINT y, MouseButton theButton );
	~MouseEvent(){};

	inline UINT getX() const { return mX; };
	inline UINT getY() const { return mY; };
	inline MouseButton getButton() const { return mButton; };


private:
	UINT mX, mY;
	MouseButton mButton;
};

