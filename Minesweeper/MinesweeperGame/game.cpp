#include <conio.h>
#include "Game.h"
#include "AIHolder.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "TaskCollection.h"
#include "WaitTask.h"
#include "FireEventTask.h"
#include "WaitAndFireEventTask.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsColor.h"
#include "GraphicsFont.h"
#include "GraphicsSprite.h"
#include "GraphicsGrid.h"
#include "InputSystem.h"
#include "DataRepository.h"
#include "StatTracker.h"
#include "GameSequencer.h"

#include "..\GameLib\EventSystem.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\PickCellEvent.h"
#include "..\GameLib\MouseEvent.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\CellClickTransaction.h"
#include "..\GameLib\Timer.h"

#include <assert.h>
#include <string>
#include <fstream>

using namespace std;

Game* gpGame = NULL;

Game::Game()
	:mpState(NULL)
	,mpPlayer(NULL)
	,mpGraphicsGrid(NULL)
	,mpTasks(NULL)
	,mpInputSystem(NULL)
	,mpSequencer(NULL)
	,mInitialized(false)
	,mKeepRunning(true)
{
}

Game::~Game()
{
	cleanup();
}

void Game::init()
{
	assert( mInitialized == false );
	mpState = new GameState;
	TransactionHandler::createTransactionHandler( GameState::handleTransaction, mpState );

	mpSequencer = new GameSequencer;

	loadAIHolders( gpDataRepository->getEntry(DataKeyEnum::DLL_FILENAME).getStringVal() );

	EventSystem::createEventSystem();
	registerCallbacks();

	mpTasks = new TaskCollection;

	GraphicsSystem::createGraphicsSystem();

	mpInputSystem = new InputSystem;
	gpStatTracker->reportStatsHeaders();
}

void Game::cleanup()
{
	mpInputSystem->cleanup();
	delete mpInputSystem;
	mpInputSystem = NULL;

	delete mpGraphicsGrid;
	mpGraphicsGrid = NULL;

	GraphicsSystem::deleteGraphicsSystem();

	delete mpTasks;
	mpTasks = NULL;

	unregisterCallbacks();
	mInitialized = false;
	mKeepRunning = false;
	TransactionHandler::destroyTransactionHandler();

	delete mpSequencer;
	mpSequencer = NULL;

	for( size_t i=0; i<mpAIHolders.size(); i++ )
	{
		delete mpAIHolders[i];
	}
	mpAIHolders.clear();

	delete mpPlayer;
	mpPlayer = NULL;

	delete mpState;
	mpState = NULL;

	EventSystem::destroyEventSystem();
}

void Game::doLoop()
{
	for (UINT i = 0; i < mpAIHolders.size(); i++)
	{
		gpStatTracker->resetAllStats();

		mpPlayer = new AIPlayer(mpAIHolders[i]);
		//mpPlayer = new HumanPlayer("Dean", TransactionHandler::getHandler());
		mpSequencer->init();
		//mpSequencer->setNextGameParams();

		UINT width = gpDataRepository->getEntry(DataKeyEnum::WIDTH_IN_CELLS).getUIntVal();
		UINT height = gpDataRepository->getEntry(DataKeyEnum::HEIGHT_IN_CELLS).getUIntVal();
		UINT mines = gpDataRepository->getEntry(DataKeyEnum::NUM_MINES).getUIntVal();
		UINT maxRewardTime = gpDataRepository->getEntry(DataKeyEnum::MAX_REWARD_TIME).getUIntVal();

		mpState->init(width, height, mines, maxRewardTime);
		mInitialized = true;
		mKeepRunning = true;

		UINT screenWidth = gpDataRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal();
		UINT screenHeight = gpDataRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal();

		GraphicsSystem::getGraphicsSystem()->init(screenWidth, screenHeight, false);
		//mpGraphicsGrid = new GraphicsGrid(mpState->getGrid(), screenWidth, screenHeight, 0, 0);
		mpGraphicsGrid = NULL;

		mpInputSystem->init();

		//gpStatTracker->addToStat(StatEnum::GAMES_SIMULATED_LARGE, 1);

		mpPlayer->setActive(true);
		//mpTasks->addTask(new FireEventTask(PICK_A_CELL_EVENT));
		mpTasks->addTask(new FireEventTask(INIT_EVENT));
		mpTasks->addTask(new FireEventTask(GAME_RESET_EVENT));
		Timer loopTimer;


		while (mKeepRunning)
		{
			loopTimer.start();
		
			TransactionHandler::getHandler()->processAllTransactions();
			mpPlayer->update();
		
			mpTasks->update();
			mpInputSystem->update();
		
			GraphicsSystem::clearBackBuffer(COLOR_BLACK);
			if (mpGraphicsGrid)
			{
				mpGraphicsGrid->draw();
			}
			GraphicsSystem::flipDisplay();
		
			//cout << loopTimer.getElapsedTime() << endl;
			double target = gpDataRepository->getEntry(DataKeyEnum::TARGET_FRAME_TIME).getDoubleVal();
			loopTimer.sleepUntilElapsed(target);
			//cout << loopTimer.getElapsedTime() << endl;
		
		}
		//gpStatTracker->logString(mpPlayer->getName() + "\n");
		gpStatTracker->logString(mpPlayer->getName());
		gpStatTracker->reportStats();

		EventSystem::getEventSystem()->fireEvent(Event(CLEANUP_EVENT));

		mpPlayer->setActive(false);
		delete mpPlayer;
		mpPlayer = NULL;

		mpInputSystem->cleanup();
		delete mpGraphicsGrid;
		mpGraphicsGrid = NULL;
		GraphicsSystem::getGraphicsSystem()->cleanup();
	}
}

void Game::processTransactions()
{
	TransactionHandler::getHandler()->processAllTransactions();
}

void Game::loadAIHolders(const string& controllerFileName)
{
	ifstream controllerFile(controllerFileName);
	if( controllerFile.good() )
	{
		int playerCount = 0;
		while( !controllerFile.eof() )
		{
			string playerName;
			getline( controllerFile, playerName );
			string dllPath;
			getline( controllerFile, dllPath );

			AIHolder* pHolder = new AIHolder( playerName, dllPath );
			mpAIHolders.push_back( pHolder );
			cout << playerName << endl;
			playerCount++;
		}
	}
}

AIHolder* Game::getAIHolder(UINT index) const
{
	assert( index < mpAIHolders.size() );

	if( index >= mpAIHolders.size() )
		return NULL;

	return mpAIHolders[index];
}

void Game::registerCallbacks()
{
	EventSystem::getEventSystem()->registerCallback( MINE_HIT_EVENT, mineHitCallback );
	EventSystem::getEventSystem()->registerCallback( CELL_REVEALED_EVENT, cellRevealedCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_WON_EVENT, gameWonCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_QUIT_EVENT, gameQuitCallback );
	EventSystem::getEventSystem()->registerCallback( MOUSE_CLICKED_EVENT, mouseClickedCallback );
	EventSystem::getEventSystem()->registerCallback( GAME_RESET_EVENT, gameResetCallback );
}

void Game::unregisterCallbacks()
{
	EventSystem::getEventSystem()->unregisterCallback( MINE_HIT_EVENT, mineHitCallback );
	EventSystem::getEventSystem()->unregisterCallback( CELL_REVEALED_EVENT, cellRevealedCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_WON_EVENT, gameWonCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_QUIT_EVENT, gameQuitCallback );
	EventSystem::getEventSystem()->unregisterCallback( MOUSE_CLICKED_EVENT, mouseClickedCallback );
	EventSystem::getEventSystem()->unregisterCallback( GAME_RESET_EVENT, gameResetCallback );
}

void Game::mineHitCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	cout << "KABOOM!!!\n";

	int score = gpGame->getGameState()->calcScore(false);

	gpGame->getGameState()->getGrid()->revealAll();
	//queue up next task
	gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::GAME_RESET_PAUSE).getUIntVal(), GAME_RESET_EVENT) );
	
	//track stats
	trackStats(score, false);
}

void Game::cellRevealedCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	//queue up next task
	gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), PICK_A_CELL_EVENT) );
}

void Game::gameWonCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	cout << "You Win!!!!\n";

	int score = gpGame->getGameState()->calcScore(true);

	gpGame->getGameState()->getGrid()->revealAll();
	//queue up next task
	gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::GAME_RESET_PAUSE).getUIntVal(), GAME_RESET_EVENT) );
	
	//track stats
	trackStats(score, true);
}

void Game::trackStats(int score, bool wonGame)
{
	int time = gpGame->getGameState()->getGameTime();

	GameSequencer::GameType type = gpGame->mpSequencer->getCurrentGameType();
	if (type == GameSequencer::LARGE)
	{
		if (wonGame)
		{
			gpStatTracker->addToStat(StatEnum::GAMES_WON_LARGE, 1);
		}
		else
		{
			gpStatTracker->addToStat(StatEnum::GAMES_LOST_LARGE, 1);
		}
		
		gpStatTracker->addToStat(StatEnum::GAMES_SIMULATED_LARGE, 1);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_LARGE, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_OVERALL, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_LARGE, time);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_OVERALL, time);
	}
	else if (type == GameSequencer::MEDIUM)
	{
		if (wonGame)
		{
			gpStatTracker->addToStat(StatEnum::GAMES_WON_MEDIUM, 1);
		}
		else
		{
			gpStatTracker->addToStat(StatEnum::GAMES_LOST_MEDIUM, 1);
		}

		gpStatTracker->addToStat(StatEnum::GAMES_SIMULATED_MEDIUM, 1);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_MEDIUM, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_OVERALL, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_MEDIUM, time);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_OVERALL, time);
	}
	else if (type == GameSequencer::SMALL)
	{
		if (wonGame)
		{
			gpStatTracker->addToStat(StatEnum::GAMES_WON_SMALL, 1);
		}
		else
		{
			gpStatTracker->addToStat(StatEnum::GAMES_LOST_SMALL, 1);
		}

		gpStatTracker->addToStat(StatEnum::GAMES_SIMULATED_SMALL, 1);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_SMALL, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_SCORE_OVERALL, score);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_SMALL, time);
		gpStatTracker->addToStat(StatEnum::TOTAL_TIME_OVERALL, time);
	}
}

void Game::gameQuitCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	cout << "You Quitter!\n";
	gpGame->mKeepRunning = false;
}

void Game::mouseClickedCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(theEvent);
	cout << "Mouse clicked " << mouseEvent.getX() << " " << mouseEvent.getY() << endl;
	UINT index = gpGame->mpGraphicsGrid->getIndexFromPixelLoc( mouseEvent.getX(),  mouseEvent.getY() );
	cout << "Grid index=" << index << endl;
	if( index != UINT_MAX )
	{
		pHandler->postTransaction( new CellClickTransaction(index) );

	}
}

void Game::gameResetCallback(const Event& theEvent, TransactionHandler* pHandler)
{
	cout << "resetting new level...\n";

	bool shouldContinue = gpGame->mpSequencer->setNextGameParams();

	if( shouldContinue )
	{
		gpGame->getGameState()->initGrid( gpDataRepository->getEntry( DataKeyEnum::NUM_MINES ).getUIntVal() );

		UINT width = gpDataRepository->getEntry(DataKeyEnum::WIDTH_IN_CELLS).getUIntVal();
		UINT height = gpDataRepository->getEntry(DataKeyEnum::HEIGHT_IN_CELLS).getUIntVal();
		UINT num = gpDataRepository->getEntry(DataKeyEnum::NUM_MINES).getUIntVal();
		UINT maxRewardTime = gpDataRepository->getEntry(DataKeyEnum::MAX_REWARD_TIME).getUIntVal();
		gpGame->resetGameState( width, height, num, maxRewardTime );
		//queue up next task
		gpGame->mpTasks->addTask( new WaitAndFireEventTask( gpDataRepository->getEntry(DataKeyEnum::FIRE_EVENT_PAUSE).getUIntVal(), PICK_A_CELL_EVENT) );
		
	}
	else
	{
		EventSystem::getEventSystem()->fireEvent( Event( GAME_QUIT_EVENT ) );
	}
}

void Game::resetGameState( UINT width, UINT height, UINT numMines, UINT maxRewardTime )
{
	if (mpGraphicsGrid)
	{
		delete mpGraphicsGrid;
	}
	mpGraphicsGrid = NULL;
	mpState->cleanup();
	mpState->init( width, height, numMines, maxRewardTime );
	UINT screenWidth = gpDataRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal();
	UINT screenHeight = gpDataRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal();

	mpGraphicsGrid = new GraphicsGrid( mpState->getGrid(), screenWidth, screenHeight, 0, 0 );
}
