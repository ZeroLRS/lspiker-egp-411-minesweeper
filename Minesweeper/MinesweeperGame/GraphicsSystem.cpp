#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsFont.h"
#include "GraphicsSprite.h"
#include "Renderer.h"
#include "GraphicsFont.h"

GraphicsSystem* GraphicsSystem::mspInstance = NULL;

GraphicsSystem::GraphicsSystem()
	:mpWindow(NULL)
	, mpRenderer(NULL)
	, mpBackBuffer(NULL)
	, mWidth(0)
	, mHeight(0)
	, mInitted(false)
{
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

bool GraphicsSystem::init(int width, int height, bool fullscreen)
{
	mWidth = width;
	mHeight = height;
	mFullScreen = fullscreen;

	//init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//create window - TODO: make fullscreen actually fullscreen!
	mpWindow = SDL_CreateWindow("TEST APP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (mpWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mpRenderer = new Renderer(mpWindow);
	mpBackBuffer = mpRenderer->makeBackBuffer();

	//enable PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//enable TTF support
	if (TTF_Init() < 0)
	{
		printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
		return false;
	}

	//looks like a memory leak, but isn't
	GraphicsFont* pFont = new GraphicsFont(ASSET_FOLDER + "cour_0.png", ASSET_FOLDER + "cour.fnt");
	gpGame->setFontID(pFont->getID());

	mInitted = true;
	return true;
}

void GraphicsSystem::cleanup()
{
	if (mInitted)
	{
		GraphicsBuffer::deleteAllGraphicsBuffers();
		mpBackBuffer = NULL;

		delete mpRenderer;
		mpRenderer = NULL;

		SDL_DestroyWindow(mpWindow);
		mpWindow = NULL;

		//Quit SDL subsystems
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		mInitted = false;
	}
}

void GraphicsSystem::deleteGraphicsSystem()
{
	GraphicsBuffer::deleteAllGraphicsBuffers();

	GraphicsFont::deleteAllFonts();

	delete mspInstance;
	mspInstance = NULL;
}

void GraphicsSystem::flip()
{
	//present renderer
	SDL_RenderPresent(mpRenderer->mpRenderer);
	SDL_SetRenderDrawColor(mpRenderer->mpRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mpRenderer->mpRenderer);
}

void GraphicsSystem::wrapCoordinates(Vector2D& vector)
{
	if (vector.getX() < 0.0f)
	{
		vector.setX(getWidth() - 1.0f);
	}
	else if (vector.getX() >= getWidth())
	{
		vector.setX(0.0f);
	}

	if (vector.getY() < 0.0f)
	{
		vector.setY(getHeight() - 1.0f);
	}
	else if (vector.getY() >= getHeight())
	{
		vector.setY(0.0f);
	}
}

void GraphicsSystem::draw(const GraphicsSprite& aSprite, float dx, float dy, float rotationInRadians /*= 0*/, int flags /*= 0*/)
{
	draw(*getBackBuffer(), aSprite, dx, dy, rotationInRadians, flags);
}

void GraphicsSystem::draw(GraphicsBuffer& dest, const GraphicsSprite& aSprite, float dx, float dy, float rotationInRadians /*= 0*/, int flags /*= 0*/)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);

	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_Rect srcRect;
	srcRect.w = (int)aSprite.getWidth();
	srcRect.h = (int)aSprite.getHeight();
	srcRect.x = (int)aSprite.getX();
	srcRect.y = (int)aSprite.getY();
	SDL_Rect destRect;
	destRect.w = (int)srcRect.w;
	destRect.h = (int)srcRect.h;
	destRect.x = (int)dx;
	destRect.y = (int)dy;
	double rotationInDegrees = rotationInRadians * (180.0 / M_PI);
	SDL_RenderCopyEx(pRenderer, aSprite.getBuffer()->mpTexture, &srcRect, &destRect, rotationInDegrees, NULL, SDL_FLIP_NONE);

	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::writeText(GraphicsFont& font, float dx, float dy, const std::string& text, const GraphicsColor& color)
{
	writeText(*(getBackBuffer()), font, dx, dy, text, color);
}

void GraphicsSystem::writeText(GraphicsBuffer& dest, GraphicsFont& font, float dx, float dy, const std::string& text, const GraphicsColor& color)
{
	// Set the new render target
	SDL_Texture* pOldTarget = setRenderTarget(dest);

	// Tint the texture the desired color
	SDL_SetTextureColorMod(font.mpCharTexture, color.r, color.g, color.b);

	SDL_Rect targetRect;
	targetRect.x = static_cast<int>(dx);
	targetRect.y = static_cast<int>(dy);

	// Loop through every character and render it to the back buffer
	for (unsigned int i = 0; i < text.size(); i++)
	{
		int charID = text.at(i);

		CharData* data = font.getCharData(charID);
		if (data == nullptr) continue;

		// Place the letter in the proper position
		targetRect.y += data->mYOffset;
		targetRect.x += data->mXOffset;
		targetRect.w = data->mLoc.w;
		targetRect.h = data->mLoc.h;

		// If this is not the first letter, check for kernings
		if (i > 0)
		{
			auto ret = font.mKernings.equal_range(charID);
			int prevCharID = text.at(i - 1);
			for (auto it = ret.first; it != ret.second; ++it)
			{
				if (it->second->mPreviousID == prevCharID)
				{
					targetRect.x += it->second->mAdjustment;
					break;
				}
			}
		}

		// Render onto the render target
		SDL_RenderCopy(mpRenderer->mpRenderer, font.mpCharTexture, &data->mLoc, &targetRect);
		
		// Undo the y offset for the next letter
		targetRect.y -= data->mYOffset;
		// Each letter keep moving the x position forward
		targetRect.x += data->mXAdvance - data->mXOffset;
	}

	// restore the old render target
	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::clear(GraphicsBuffer& buffer, const GraphicsColor& color)
{
	SDL_Texture* pOldTarget = setRenderTarget(buffer);

	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_SetRenderTarget(pRenderer, buffer.mpTexture);
	SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(pRenderer);

	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::fillRegion(GraphicsBuffer& buffer, const Vector2D& ul, const Vector2D& lr, const GraphicsColor& color)
{
	SDL_Texture* pOldTarget = setRenderTarget(buffer);

	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(pRenderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, color.a);

	SDL_Rect destRect;
	destRect.x = (int)ul.getX();
	destRect.y = (int)ul.getY();
	destRect.w = (int)lr.getX() - destRect.x;
	destRect.h = (int)lr.getY() - destRect.y;

	SDL_RenderFillRect(pRenderer, &destRect);

	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);

	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::setWindowTitle(const std::string& newTitle)
{
	SDL_SetWindowTitle(mpWindow, newTitle.c_str());
}

SDL_Texture* GraphicsSystem::loadTexture(const std::string& filename)
{
	SDL_Surface* pSurface = IMG_Load(filename.c_str());

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(mpRenderer->mpRenderer, pSurface);
	SDL_FreeSurface(pSurface);
	return pTexture;
}

SDL_Texture* GraphicsSystem::createTexture(int width, int height)
{
	SDL_Texture* pTexture = SDL_CreateTexture(mpRenderer->mpRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
	return pTexture;
}

void GraphicsSystem::clearBackBuffer(const GraphicsColor& theColor)
{
	mspInstance->clear(*(mspInstance->getBackBuffer()), theColor);
}

void GraphicsSystem::flipDisplay()
{
	mspInstance->flip();
}

void GraphicsSystem::drawSprite(const GraphicsSprite& sprite, UINT dx, UINT dy, float rotationInRadians /*= 0.0f*/, int flags /*= 0*/, GraphicsBuffer* pDest /*= NULL*/)
{
	if (pDest == NULL)
	{
		mspInstance->draw(sprite, (float)dx, (float)dy, rotationInRadians, flags);
	}
	else
	{
		mspInstance->draw(*pDest, sprite, (float)dx, (float)dy, rotationInRadians, flags);
	}

}

void GraphicsSystem::drawText(GraphicsFont* pFont, UINT dx, UINT dy, const GraphicsColor& theColor, const std::string& text, GraphicsBuffer* pDest /*= NULL*/)
{
	if (pDest == NULL)
	{
		mspInstance->writeText(*pFont, (float)dx, (float)dy, text, theColor);
	}
	else
	{
		mspInstance->writeText(*pDest, *pFont, (float)dx, (float)dy, text, theColor);
	}
}

GraphicsBuffer* GraphicsSystem::loadGraphicsBuffer(const std::string& filename)
{
	SDL_Texture* pTexture = mspInstance->loadTexture(filename);
	return new GraphicsBuffer(pTexture);
}

GraphicsBuffer* GraphicsSystem::createGraphicsBuffer(UINT width, UINT height, const GraphicsColor& theColor /*= COLOR_BLACK*/)
{
	SDL_Texture* pTexture = mspInstance->createTexture(width, height);
	GraphicsBuffer* pBuffer = new GraphicsBuffer(pTexture);
	mspInstance->clear(*pBuffer, theColor);
	return pBuffer;
}

SDL_Texture* GraphicsSystem::setRenderTarget(GraphicsBuffer& buffer)
{
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_Texture* pOldTarget = SDL_GetRenderTarget(pRenderer);
	SDL_SetRenderTarget(pRenderer, buffer.mpTexture);
	return pOldTarget;
}

void GraphicsSystem::restoreRenderTarget(SDL_Texture* oldTarget)
{
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_SetRenderTarget(pRenderer, oldTarget);
}
