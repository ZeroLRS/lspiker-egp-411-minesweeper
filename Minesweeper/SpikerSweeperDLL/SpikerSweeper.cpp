#include "SpikerSweeper.h"

SpikerSweeper::SpikerSweeper(int width, int height, int numMines)
{
	mWidth = width;
	mHeight = height;
	mNumMines = numMines;
	mNumMarkedMines = 0;

	//mpMineLocations = new int[numMines];
}

SpikerSweeper::~SpikerSweeper()
{
	//delete mpMineLocations;
	//delete mpTheView;
}

// Count the number of mines surrounding our cell
int SpikerSweeper::getAdjacentMarkedMines(int origin)
{
	int mineCounter = 0;
	std::vector<UINT> adjacentSquares = mpTheView->getAdjacentCellIndices(origin);

	for each (UINT square in adjacentSquares)
	{
		if (isCellMine(square))
			mineCounter++;
	}

	return mineCounter;
}

// Old pre-getAdjacentCellIndicies version
/*
int SpikerSweeper::getAdjacentMarkedMines(int origin)
{
	int mineCount = 0;
	int ourRow = origin / mWidth;
	int ourCol = origin % mWidth;
	bool up, down, left, right;

	// Do we need to go up?
	up = ourRow > 0; // Don't go up if we are already at the top

	// Do we need to go down?
	down = ourRow < mHeight - 1; // Don't go down if we are already at the bottom

	// Do we need to go left?
	left = ourCol > 0; // Don't go left if we are at that side

	// Do we need to go right?
	right = ourCol < mWidth - 1; // Don't go right if we are at that side

	/* Follow this order for evaluation (# is the current location
	 *
	 *  1 2 3
	 *  4 # 5
	 *  6 7 8
	 *
	 *//*

	if (up && left && isCellMine(mWidth * ourRow - 1 + ourCol -1))
		mineCount++;

	if (up && isCellMine(mWidth * ourRow - 1 + ourCol))
		mineCount++;

	if (up && right && isCellMine(mWidth * ourRow - 1 + ourCol + 1))
		mineCount++;

	if (left && isCellMine(mWidth * ourRow + ourCol - 1))
		mineCount++;

	if (right && isCellMine(mWidth * ourRow + ourCol + 1))
		mineCount++;

	if (down && left && isCellMine(mWidth * ourRow + 1 + ourCol - 1))
		mineCount++;

	if (down && isCellMine(mWidth * ourRow + 1 + ourCol))
		mineCount++;

	if (down && right && isCellMine(mWidth * ourRow + 1 + ourCol + 1))
		mineCount++;

	return mineCount;
}*/

// Count the number of unopened mines surrounding our cell
int SpikerSweeper::getAdjacentUnopened(int origin)
{
	int unopenedCounter = 0;
	std::vector<UINT> adjacentSquares = mpTheView->getAdjacentCellIndices(origin);

	for each (UINT square in adjacentSquares)
	{
		if (!mpTheView->getCell(square).isRevealed())
			unopenedCounter++;
	}

	return unopenedCounter;
}

// Old pre-getAdjacentCellIndicies version
/*
int SpikerSweeper::getAdjacentUnopened(int origin)
{
	int unopenedCount = 0;
	int ourRow = origin / mWidth;
	int ourCol = origin % mWidth;
	bool up, down, left, right;

	// Do we need to go up?
	up = ourRow > 0; // Don't go up if we are already at the top

	// Do we need to go down?
	down = ourRow < mHeight - 1; // Don't go down if we are already at the bottom

	// Do we need to go left?
	left = ourCol > 0; // Don't go left if we are at that side

	// Do we need to go right?
	right = ourCol < mWidth - 1; // Don't go right if we are at that side

	/* Follow this order for evaluation (# is the current location
	*
	*  1 2 3
	*  4 # 5
	*  6 7 8
	*
	*//*

	if (up && left && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol - 1).isRevealed())
		unopenedCount++;

	if (up && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol).isRevealed())
		unopenedCount++;

	if (up && right && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol + 1).isRevealed())
		unopenedCount++;

	if (left && !mpTheView->getCell(mWidth * ourRow + ourCol - 1).isRevealed())
		unopenedCount++;

	if (right && !mpTheView->getCell(mWidth * ourRow + ourCol + 1).isRevealed())
		unopenedCount++;

	if (down && left && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol - 1).isRevealed())
		unopenedCount++;

	if (down && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol).isRevealed())
		unopenedCount++;

	if (down && right && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol + 1).isRevealed())
		unopenedCount++;

	return unopenedCount;
}*/

// Getting specific square data
int SpikerSweeper::getRandomUnopenedNotMine()
{
	while (1) // Don't do while 1 loops kids. // TODO: Remove while 1 loop
	{
		int index = rand() % mpTheView->getNumCells();
		HiddenCell cell = mpTheView->getCell(index);
		if (!cell.isRevealed() && !isCellMine(index))
		{
			return index;
		}
	}
}

int SpikerSweeper::getNextSafeSquare()
{
	//If we have nothing, return something random
	if (mSafeLocations.size() <= 0)
	{
		std::cout << "This shouldn't happen ever..." << std::endl;
		return getRandomUnopenedNotMine();
	}

	// Get, remove, and return the last element in the vector.
	int safeLocation = mSafeLocations.back();
	mSafeLocations.pop_back();
	return safeLocation;
}

int SpikerSweeper::getNumMarkedMines()
{
	return mNumMarkedMines;
}

int SpikerSweeper::getNumSafeSquares()
{
	return mSafeLocations.size();
}

bool SpikerSweeper::isCellMine(int index)
{
	// Make sure the cell is unrevealed...
	mpTheView->getCell(index).isRevealed();

	// Check if the current index is on the list of mines
	for each (int i in mpMineLocations)
	{
		if (i == index)
		{
			//std::cout << "Cell " << index << " IS mine!" << std::endl;
			return true;
		}
	}

	return false;
}

// Adding square data
void SpikerSweeper::addMineLocation(int mineLocation)
{
	// Prevent duplication
	for each (int i in mpMineLocations)
	{
		if (i == mineLocation)
		{
			//std::cout << "Prevented duplicate mine entry." << std::endl;
			return;
		}
	}

	// Add the new mine location and increment the counter
	mpMineLocations.push_back(mineLocation);
	//std::cout << "Added MINE at location: " << mineLocation << std::endl;
}

void SpikerSweeper::addSafeLocation(int safeLocation)
{
	// Prevent duplication
	for each (int i in mSafeLocations)
	{
		if (i == safeLocation)
			return;
	}
	mSafeLocations.push_back(safeLocation);
	//std::cout << "Added SAFE at location: " << safeLocation << std::endl;
}

void SpikerSweeper::calculateBoardState()
{
	int currentSafes = -1;
	int prevSafes = -2;
	int currentMines = -1;
	int prevMines = -2;

	//while (currentSafes != prevSafes || currentMines != prevMines)
	//{
		//while (currentMines != prevMines)
		//{
		detectMines();

		prevMines = currentMines;
		currentMines = getNumMarkedMines();
		//}

		detectSafes();

		prevSafes = currentSafes;
		currentSafes = mSafeLocations.size();

		//detectMines();

		//prevMines = currentMines;
		//currentMines = getNumMarkedMines();

	//}
}

// For all squares with value n, if they touch exactly n unopened squares, all of those squares must be mines
void SpikerSweeper::detectMines()
{
	for (int i = 0; i < mWidth * mHeight; i++)
	{
		if (mpTheView->getCell(i).isRevealed() && mpTheView->getNumAdjacentMines(i) > 0)
		{
			// Square value == adjacent unopened
			if (mpTheView->getNumAdjacentMines(i) == getAdjacentUnopened(i))
			{
				// All currently unopened mines adjacent to i are mines and should be marked.
				markUnopenedAdjacent(i);
			}
		}
	}
}

// for all squares with value n, if they touch n mines, all other adjacent squares must NOT be mines
void SpikerSweeper::detectSafes()
{
	for (int i = 0; i < mWidth * mHeight; i++)
	{
		if (mpTheView->getCell(i).isRevealed() && mpTheView->getNumAdjacentMines(i) > 0)
		{
			// Square value == adjacent mines
			//std::cout << "gnam: " << mpTheView->getNumAdjacentMines(i) << " gamm: " << getAdjacentMarkedMines(i) << std::endl;
			if (mpTheView->getNumAdjacentMines(i) == getAdjacentMarkedMines(i))
			{
				// All currently unopened, non-mine squares adjacent to i are safe and should be added.
				//std::cout << "Marking squares adjacent to: " << i << " SAFE" << std::endl;
				safeUnopenedNonMineAdjacent(i);
			}
		}
	}
}

// Mark all nearby cells that aren't opened as mines
void SpikerSweeper::markUnopenedAdjacent(int origin)
{
	std::vector<UINT> adjacentSquares = mpTheView->getAdjacentCellIndices(origin);

	for each (UINT square in adjacentSquares)
	{
		if (!mpTheView->getCell(square).isRevealed())
			addMineLocation(square);
	}
}

// Old pre-getAdjacentCellIndicies version
/*
void SpikerSweeper::markUnopenedAdjacent(int origin)
{
	int ourRow = origin / mWidth;
	int ourCol = origin % mWidth;
	bool up, down, left, right;

	// Do we need to go up?
	up = ourRow > 0; // Don't go up if we are already at the top

	// Do we need to go down?
	down = ourRow < mHeight - 1; // Don't go down if we are already at the bottom

	// Do we need to go left?
	left = ourCol > 0; // Don't go left if we are at that side

	// Do we need to go right?
	right = ourCol < mWidth - 1; // Don't go right if we are at that side

	/* Follow this order for evaluation (# is the current location
	*
	*  1 2 3
	*  4 # 5
	*  6 7 8
	*
	*//*

	if (up && left && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol - 1).isRevealed())
		addMineLocation(mWidth * ourRow - 1 + ourCol - 1);

	if (up && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol).isRevealed())
		addMineLocation(mWidth * ourRow - 1 + ourCol);

	if (up && right && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol + 1).isRevealed())
		addMineLocation(mWidth * ourRow - 1 + ourCol + 1);

	if (left && !mpTheView->getCell(mWidth * ourRow + ourCol - 1).isRevealed())
		addMineLocation(mWidth * ourRow + ourCol - 1);

	if (right && !mpTheView->getCell(mWidth * ourRow + ourCol + 1).isRevealed())
		addMineLocation(mWidth * ourRow + ourCol + 1);

	if (down && left && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol - 1).isRevealed())
		addMineLocation(mWidth * ourRow + 1 + ourCol - 1);

	if (down && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol).isRevealed())
		addMineLocation(mWidth * ourRow + 1 + ourCol);

	if (down && right && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol + 1).isRevealed())
		addMineLocation(mWidth * ourRow + 1 + ourCol + 1);
}*/

// Mark all nearby cells that aren't opened and aren't mines as safe
void SpikerSweeper::safeUnopenedNonMineAdjacent(int origin)
{
	std::vector<UINT> adjacentSquares = mpTheView->getAdjacentCellIndices(origin);

	for each (UINT square in adjacentSquares)
	{
		if (!mpTheView->getCell(square).isRevealed() && !isCellMine(square))
			addSafeLocation(square);
	}
}

// Old pre-getAdjacentCellIndicies version
/*
void SpikerSweeper::safeUnopenedNonMineAdjacent(int origin)
{
	int ourRow = origin / mWidth;
	int ourCol = origin % mWidth;
	bool up, down, left, right;

	// Do we need to go up?
	up = ourRow > 0; // Don't go up if we are already at the top

	// Do we need to go down?
	down = ourRow < mHeight - 1; // Don't go down if we are already at the bottom

	// Do we need to go left?
	left = ourCol > 0; // Don't go left if we are at that side

	// Do we need to go right?
	right = ourCol < mWidth - 1; // Don't go right if we are at that side

	/* Follow this order for evaluation (# is the current location
	*
	*  1 2 3
	*  4 # 5
	*  6 7 8
	*
	*//*

	if (up && left && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol - 1).isRevealed() && !isCellMine(mWidth * ourRow - 1 + ourCol - 1))
		addSafeLocation(mWidth * ourRow - 1 + ourCol - 1);

	if (up && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol).isRevealed() && !isCellMine(mWidth * ourRow - 1 + ourCol))
		addSafeLocation(mWidth * ourRow - 1 + ourCol);

	if (up && right && !mpTheView->getCell(mWidth * ourRow - 1 + ourCol + 1).isRevealed() && !isCellMine(mWidth * ourRow - 1 + ourCol + 1))
		addSafeLocation(mWidth * ourRow - 1 + ourCol + 1);

	if (left && !mpTheView->getCell(mWidth * ourRow + ourCol - 1).isRevealed() && !isCellMine(mWidth * ourRow + ourCol - 1))
		addSafeLocation(mWidth * ourRow + ourCol - 1);

	if (right && !mpTheView->getCell(mWidth * ourRow + ourCol + 1).isRevealed() && !isCellMine(mWidth * ourRow + ourCol + 1))
		addSafeLocation(mWidth * ourRow + ourCol + 1);

	if (down && left && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol - 1).isRevealed() && !isCellMine(mWidth * ourRow + 1 + ourCol - 1))
		addSafeLocation(mWidth * ourRow + 1 + ourCol - 1);

	if (down && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol).isRevealed() && !isCellMine(mWidth * ourRow + 1 + ourCol))
		addSafeLocation(mWidth * ourRow + 1 + ourCol);

	if (down && right && !mpTheView->getCell(mWidth * ourRow + 1 + ourCol + 1).isRevealed() && !isCellMine(mWidth * ourRow + 1 + ourCol + 1))
		addSafeLocation(mWidth * ourRow + 1 + ourCol + 1);
}*/