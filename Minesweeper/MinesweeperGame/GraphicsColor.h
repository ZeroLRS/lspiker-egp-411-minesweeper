#pragma once

#include "..\gamelib\trackable.h"
#include <wtypes.h>

struct SDL_Color;

struct GraphicsColor : public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsColor(UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha = 255);//default to solid

	UCHAR r, g, b, a;

private:
	SDL_Color mapGraphicsColorToSDLColor() const;

};

const GraphicsColor COLOR_BLACK(0, 0, 0, 255);
const GraphicsColor COLOR_WHITE(255, 255, 255, 255);
const GraphicsColor COLOR_RED(200, 0, 0, 255);
const GraphicsColor COLOR_GREEN(10, 150, 10, 255);
const GraphicsColor COLOR_BLUE(0, 0, 255, 255);

