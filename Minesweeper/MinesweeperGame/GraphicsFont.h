#pragma once

#include "..\gamelib\trackable.h"
#include <wtypes.h>
#include <map>
#include <string>
#include <SDL.h>


struct CharData
{
	SDL_Rect mLoc;
	int mXOffset;
	int mYOffset;
	int mXAdvance;
};

struct Kerning
{
	int mPreviousID;
	int mAdjustment;
};

class GraphicsFont;

typedef UINT FontID;
typedef std::map<FontID, GraphicsFont*> FontContainer;


class GraphicsFont : public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsFont(const std::string& imgFilename, const std::string& descFilename);
	~GraphicsFont();

	inline FontID getID() const { return mID; };
	inline size_t getSize() const { return mSize; };

	static GraphicsFont* getFont(const FontID& id);
	static bool fontExists(const FontID& id);
	static void deleteAllFonts();

private:
	static FontID msNextID;
	static FontContainer msFonts;

	std::map<int, CharData*> mCharDescriptors;
	std::multimap<int, Kerning*> mKernings;

	// A Texture containing all of the characters found in this font
	SDL_Texture* mpCharTexture;

	CharData* getCharData(int id) const;
	void getTextureDimensions(const std::string& text, int& width, int& height) const;

	FontID mID;
	size_t mSize;
};

