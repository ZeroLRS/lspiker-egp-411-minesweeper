#include "GraphicsColor.h"
#include <SDL_image.h>


GraphicsColor::GraphicsColor(UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
	:r(red)
	, g(green)
	, b(blue)
	, a(alpha)
{
}

SDL_Color GraphicsColor::mapGraphicsColorToSDLColor() const
{
	SDL_Color theColor = { r, g, b, a };
	return theColor;
}
