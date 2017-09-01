#include "GraphicsFont.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsColor.h"

#include <cassert>
#include <SDL_image.h>
#include "Renderer.h"
#include "tinyxml2/tinyxml2.h"

using namespace std;

FontID GraphicsFont::msNextID = 0;
FontContainer GraphicsFont::msFonts;

/**
 * \brief Constructor for GraphicsFont
 * \param imgFilename The filename of the image containing all of this font's characters
 * \param descFilename The filename of the descriptor file (.fnt) that pairs with this font's image file. See http://www.angelcode.com/products/bmfont/ for different fonts
 */
GraphicsFont::GraphicsFont(const std::string& imgFilename, const std::string& descFilename)
	:mID(msNextID++)
	, mSize(0)
{
	assert(!fontExists(mID));
	msFonts[mID] = this;

	SDL_Surface* pFontSurface = IMG_Load(imgFilename.c_str());
	mpCharTexture = SDL_CreateTextureFromSurface(GraphicsSystem::getGraphicsSystem()->getRenderer()->mpRenderer, pFontSurface);
	SDL_FreeSurface(pFontSurface);

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->LoadFile(descFilename.c_str());

	// Navigate to the correct element in the xml file
	tinyxml2::XMLElement* dataElement = doc->FirstChildElement()->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
	tinyxml2::XMLElement* currElement = dataElement->FirstChildElement();

	// Grab all required data from each element
	while (currElement != nullptr)
	{
		int id = currElement->IntAttribute("id");
		CharData* data = new CharData();
		data->mLoc.x = currElement->IntAttribute("x");
		data->mLoc.y = currElement->IntAttribute("y");
		data->mLoc.w = currElement->IntAttribute("width");
		data->mLoc.h = currElement->IntAttribute("height");

		data->mXOffset = currElement->IntAttribute("xoffset");
		data->mYOffset = currElement->IntAttribute("yoffset");
		data->mXAdvance = currElement->IntAttribute("xadvance");

		mCharDescriptors.emplace(id, data);

		currElement = currElement->NextSiblingElement(); // increment
	}

	// Navigate to the kerning element
	tinyxml2::XMLElement* kerningElement = dataElement->NextSiblingElement();
	currElement = kerningElement->FirstChildElement();

	// Grab all required data from each element
	while(currElement != nullptr)
	{
		Kerning* k = new Kerning();

		k->mPreviousID = currElement->IntAttribute("first");
		k->mAdjustment = currElement->IntAttribute("amount");

		int charID = currElement->IntAttribute("second");

		mKernings.emplace(charID, k);

		currElement = currElement->NextSiblingElement(); // increment
	}

}


GraphicsFont::~GraphicsFont()
{
	//remove from container
	msFonts.erase(mID);

	for (auto it = mCharDescriptors.begin(); it != mCharDescriptors.end(); ++it)
	{
		delete it->second;
	}

	for (auto it = mKernings.begin(); it != mKernings.end(); ++it)
	{
		delete it->second;
	}
	if (mpCharTexture != nullptr) SDL_DestroyTexture(mpCharTexture);
}

bool GraphicsFont::fontExists(const FontID& id)
{
	FontContainer::const_iterator iter = msFonts.find(id);
	if (iter == msFonts.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

GraphicsFont* GraphicsFont::getFont(const FontID& id)
{
	if (fontExists(id))
	{
		return msFonts[id];
	}
	else
	{
		return NULL;
	}
}

void GraphicsFont::deleteAllFonts()
{
	while (msFonts.size() > 0)
	{
		FontContainer::iterator iter = msFonts.begin();
		delete iter->second;
	}
}

/**
 * \brief Accesses the CharData struct for a particular letter
 * \param id The ASCII code for the letter
 * \return The CharData for the character
 */
CharData* GraphicsFont::getCharData(int id) const
{
	CharData* data = mCharDescriptors.at(id);
	if (data == nullptr)
	{
		cout << "Trying to print a letter that does not exist in this font!\n";
	}
	return data;
}

/**
 * \brief Gets the dimensions required for a texture containing the desired text
 * \param text The desired text
 * \param width the width of the texture
 * \param height the height of the texture
 */
void GraphicsFont::getTextureDimensions(const std::string& text, int& width, int& height) const
{
	// Declare all variables
	width = height = 0;
	int letterHeight, i, length = text.length();

	// For each letter, grab charater data and determine impact on dimensions
	for (i = 0; i < length; i++)
	{
		CharData* data = getCharData(static_cast<int>(text.at(i)));
		if (data == nullptr) continue;


		// Increment the width
		width += data->mXAdvance;

		// Determine if this letter changes the height required
		letterHeight = data->mLoc.h + data->mYOffset;
		if (letterHeight > height) height = letterHeight;
	}
}
