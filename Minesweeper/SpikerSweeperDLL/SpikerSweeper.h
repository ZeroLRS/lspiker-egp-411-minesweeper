#ifndef _SPIKERSWEEPER_H
#define _SPIKERSWEEPER_H

#include <string>
#include <vector>
#include "..\GameLib\GameView.h"

class SpikerSweeper
{

public:
	// Construction / Deconstruction
	SpikerSweeper(int width, int height, int numMines);
	~SpikerSweeper();

	// GameView setting
	inline void setGameView(const GameView* theView) { mpTheView = theView; };

	int getRandomUnopenedNotMine();
	int getNextSafeSquare();

	int getNumSafeSquares();

	void calculateBoardState();

private:
	std::vector<int> mpMineLocations;
	std::vector<int> mSafeLocations;

	int mWidth, mHeight;
	int mNumMines, mNumMarkedMines;

	const GameView* mpTheView;

	// Counts
	int getAdjacentMarkedMines(int origin);
	int getAdjacentUnopened(int origin);

	// Getting specific square data
	int getNumMarkedMines();
	bool isCellMine(int index);

	// Adding square data
	void addMineLocation(int mineLocation);
	void addSafeLocation(int safeLocation);

	void detectMines();
	void detectSafes();
	void markUnopenedAdjacent(int index);
	void safeUnopenedNonMineAdjacent(int index);

};

#endif //!_SPIKERSWEEPER_H