#include "GraphicsSprite.h"


GraphicsSprite::GraphicsSprite()
	:mpBuffer(NULL)
	, mX(0)
	, mY(0)
	, mWidth(0)
	, mHeight(0)
{
}

GraphicsSprite::GraphicsSprite(GraphicsBuffer* pTheBuffer, UINT sx, UINT sy, UINT width, UINT height)
	:mpBuffer(pTheBuffer)
	, mX(sx)
	, mY(sy)
	, mWidth(width)
	, mHeight(height)
{
}

