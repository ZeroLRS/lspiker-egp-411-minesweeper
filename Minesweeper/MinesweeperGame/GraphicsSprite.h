#pragma once

#include "..\gamelib\trackable.h"
#include <wtypes.h>

class GraphicsBuffer;

class GraphicsSprite : public Trackable
{
public:
	GraphicsSprite();
	GraphicsSprite(GraphicsBuffer* pTheBuffer, UINT sx, UINT sy, UINT width, UINT height);
	~GraphicsSprite() {};

	inline GraphicsBuffer* getBuffer() const { return mpBuffer; };
	inline UINT getX() const { return mX; };
	inline UINT getY() const { return mY; };
	inline UINT getWidth() const { return mWidth; };
	inline UINT getHeight() const { return mHeight; };

private:
	GraphicsBuffer* mpBuffer;
	UINT mX, mY;
	UINT mWidth, mHeight;
};

