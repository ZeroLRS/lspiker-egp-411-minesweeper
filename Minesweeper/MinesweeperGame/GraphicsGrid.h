#pragma once

#include "..\gamelib\trackable.h"
#include <wtypes.h>

class GraphicsSprite;
class Grid;
class Cell;

class GraphicsGrid : public Trackable
{
public:
	GraphicsGrid(Grid* pGrid, size_t pixelWidth, size_t pixelHeight, UINT xLoc, UINT yLoc);
	virtual ~GraphicsGrid();

	void draw() const;
	UINT getIndexFromPixelLoc(UINT xLoc, UINT yLoc) const;

private:
	size_t mWidthInCells; //width expressed as number of cells
	size_t mHeightInCells; //height expressed as number of cells
	size_t mWidthInPixels;
	size_t mHeightInPixels;
	UINT mULPixelLocX; //upper left x coordinate
	UINT mULPixelLocY; //upper left y coordinate
	Grid* mpGrid;

	GraphicsSprite* mpUnrevealedCellImage;
	GraphicsSprite* mpMineCellImage;
	GraphicsSprite* mpRevealedCellImage;

	const size_t CELL_WIDTH;
	const size_t CELL_HEIGHT;

	void getPixelLocFromIndex(UINT index, UINT& x, UINT& y) const;
	//Cell* getCellFromPixelLoc(UINT xLoc, UINT yLoc) const;
};

