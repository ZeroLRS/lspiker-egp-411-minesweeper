#pragma once

#include "..\gamelib\trackable.h"
#include <string>
#include <map>
#include <wtypes.h>

struct GraphicsColor;
class GraphicsBuffer;
struct SDL_Texture;
struct SDL_Renderer;

const int INVALID_DIMENSION = -1;
typedef UINT BufferID;
typedef std::map<BufferID, GraphicsBuffer*> BufferContainer;

class GraphicsBuffer : public Trackable
{
public:
	//accessors
	int getWidth() const;
	int getHeight() const;
	void getDimensions(int& width, int& height) const;
	inline bool isValid() const { return mValid; };

	void setToColor(const GraphicsColor& color); //not in Gin
	inline BufferID getID() const { return mID; };

	static GraphicsBuffer* getBuffer(const BufferID& id);
	static bool bufferExists(const BufferID& id);
	static void deleteAllGraphicsBuffers();

private:
	static BufferID msNextID;
	static BufferContainer msBuffers;

	UINT mID;
	SDL_Texture* mpTexture;
	bool mValid = false;

	GraphicsBuffer(SDL_Renderer* pRenderer); //used to create the back buffer
	GraphicsBuffer(SDL_Texture* pTexture);
	~GraphicsBuffer();

	friend class GraphicsSystem;
	friend class Renderer;
};

