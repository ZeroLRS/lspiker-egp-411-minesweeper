#include <iostream>
#include <thread>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Game.h"
#include "AIHolder.h"
#include "DataRepository.h"
#include "DataLoader.h"
#include "StatTracker.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\Grid.h"
#include "..\GameLib\Cell.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\MemoryTracker.h"
#include "..\GameLib\PerformanceTracker.h"
#include "..\MinesweeperDLL\DLLCode.h"

using namespace std;

int main(int argc, char **argv)
{
	{
		PerformanceTracker performanceTracker;
		performanceTracker.startTracking("Application");

		gpDataRepository = new DataRepository;
		srand((UINT)time(NULL));
		DataLoader loader("driver.txt");
		gpStatTracker = new StatTracker("stats.txt");
		gpGame = new Game;
		gpGame->init();

		gpGame->doLoop();

		delete gpGame;

		delete gpStatTracker;
		delete gpDataRepository;

		performanceTracker.stopTracking("Application");
		cout << "Total time to run application: " << performanceTracker.getElapsedTime("Application") << " ms" << endl << endl;
	}
	gMemoryTracker.reportAllocations( cout );
	system("pause");

	return 0;
}
