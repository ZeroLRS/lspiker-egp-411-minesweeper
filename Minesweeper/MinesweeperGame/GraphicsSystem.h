#pragma once

#include <cassert>
#include <string>
#include <wtypes.h>
#include "..\gamelib\trackable.h"
#include "..\gamelib\Vector2D.h"
#include "GraphicsColor.h"
#include "InputSystem.h"

class GraphicsBuffer;
class GraphicsSprite;
class Renderer;
class GraphicsFont;
struct SDL_Window;
struct SDL_Texture;

const std::string ASSET_FOLDER = "assets/";

class GraphicsSystem :public Trackable
{
	friend void InputSystem::init();
	friend void InputSystem::cleanup();
public:
	GraphicsSystem();
	~GraphicsSystem();

	bool init(int width, int height, bool fullscreen);
	void cleanup();

	void flip();
	void wrapCoordinates(Vector2D& vector);//change the x and y values in the vector to keep them on the visible screen

	//accessors
	int getWidth() const { return mWidth; };
	int getHeight() const { return mHeight; };
	inline GraphicsBuffer* getBackBuffer() { return mpBackBuffer; };
	inline Renderer* getRenderer() { return mpRenderer; };

	//draw the contents of the sprite to the Backbuffer - may be rotated
	void draw(const GraphicsSprite& aSprite, float dx, float dy, float rotationInRadians = 0, int flags = 0);
	//draw the contents of the sprite to a GraphicsBuffer - may be rotated
	void draw(GraphicsBuffer& dest, const GraphicsSprite& aSprite, float dx, float dy, float rotationInRadians = 0, int flags = 0);
	//writeText
	void writeText(GraphicsFont& font, float dx, float dy, const std::string& text, const GraphicsColor& color);
	//writeText
	void writeText(GraphicsBuffer& dest, GraphicsFont& font, float dx, float dy, const std::string& text, const GraphicsColor& color);
	//set entire buffer to given color
	void clear(GraphicsBuffer& buffer, const GraphicsColor& color = COLOR_BLACK);
	//set a rectangular region to a color
	void fillRegion(GraphicsBuffer& buffer, const Vector2D& ul, const Vector2D& lr, const GraphicsColor& color);

	void setWindowTitle(const std::string& newTitle);

	//static functions
	inline static GraphicsSystem* getGraphicsSystem() { assert(mspInstance != NULL); return mspInstance; };
	inline static void createGraphicsSystem() { assert(mspInstance == NULL); mspInstance = new GraphicsSystem; };
	static void deleteGraphicsSystem();

	static void clearBackBuffer(const GraphicsColor& theColor = COLOR_BLACK);
	static void flipDisplay();
	static void drawSprite(const GraphicsSprite& sprite, UINT dx, UINT dy, float rotationInRadians = 0.0f, int flags = 0, GraphicsBuffer* pDest = NULL);
	static void drawText(GraphicsFont* pFont, UINT dx, UINT dy, const GraphicsColor& theColor, const std::string& text, GraphicsBuffer* pDest = NULL);
	static GraphicsBuffer* loadGraphicsBuffer(const std::string& filename);
	static GraphicsBuffer* createGraphicsBuffer(UINT width, UINT height, const GraphicsColor& theColor = COLOR_BLACK);

private:
	static GraphicsSystem* mspInstance;

	SDL_Window* mpWindow;
	Renderer* mpRenderer;
	GraphicsBuffer* mpBackBuffer;
	int mWidth;
	int mHeight;
	bool mInitted;
	bool mFullScreen;

	SDL_Texture* loadTexture(const std::string& filename);
	SDL_Texture* createTexture(int width, int height);
	SDL_Texture* setRenderTarget(GraphicsBuffer& pBuffer);//returns previous render target texture
	void restoreRenderTarget(SDL_Texture* oldTarget);
};

/* fxns that were in original GraphicsSystem that aren't anymore:
 * -drawEntireBuffer
 * -flipDisplay (replaced with flip)
 *
 * fxns that weren't in original GraphicsSystem that are here now:
 * -flip (replaces flipDisplay)
 * -wrapCoordinates
 * -
 */