#include "GameSequencer.h"
#include "DataRepository.h"


GameSequencer::GameSequencer()
{
	init();
}

bool GameSequencer::setNextGameParams()
{
	switch( mCurrentGameType )
	{
	case GameType::LARGE:
		{
			if( mNumLargeStillToPlay == 0 )
			{
				mCurrentGameType = GameType::MEDIUM;
			}
		}
	case GameType::MEDIUM:
		{
			if( mNumMediumStillToPlay == 0 )
			{
				mCurrentGameType = GameType::SMALL;
			}
		}
	case GameType::SMALL:
		{
			if( mNumSmallStillToPlay == 0 )
			{
				mCurrentGameType = GameType::END_SIMULATION;
			}
		}
	}

	if( mCurrentGameType == GameType::LARGE )
	{
		gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::WIDTH_IN_CELLS_LARGE).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::HEIGHT_IN_CELLS_LARGE).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::NUM_MINES, gpDataRepository->getEntry( DataKeyEnum::NUM_MINES_LARGE).getUIntVal());
		mNumLargeStillToPlay--;
	}
	else if( mCurrentGameType == GameType::MEDIUM )
	{
		gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::WIDTH_IN_CELLS_MEDIUM).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::HEIGHT_IN_CELLS_MEDIUM).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::NUM_MINES, gpDataRepository->getEntry( DataKeyEnum::NUM_MINES_MEDIUM).getUIntVal());
		mNumMediumStillToPlay--;
	}
	else if( mCurrentGameType == GameType::SMALL )
	{
		gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::WIDTH_IN_CELLS_SMALL).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS, gpDataRepository->getEntry( DataKeyEnum::HEIGHT_IN_CELLS_SMALL).getUIntVal() );
		gpDataRepository->addEntry( DataKeyEnum::NUM_MINES, gpDataRepository->getEntry( DataKeyEnum::NUM_MINES_SMALL).getUIntVal());
		mNumSmallStillToPlay--;
	}
	else
	{
		return false;
	}

	return true;
}

void GameSequencer::init()
{
	mNumLargeStillToPlay = gpDataRepository->getEntry(DataKeyEnum::NUM_GAMES_LARGE).getUIntVal();
	mNumMediumStillToPlay = gpDataRepository->getEntry(DataKeyEnum::NUM_GAMES_MEDIUM).getUIntVal();
	mNumSmallStillToPlay = gpDataRepository->getEntry(DataKeyEnum::NUM_GAMES_SMALL).getUIntVal();
	mCurrentGameType = GameType::LARGE;

	gpDataRepository->addEntry(DataKeyEnum::WIDTH_IN_CELLS, gpDataRepository->getEntry(DataKeyEnum::WIDTH_IN_CELLS_LARGE).getUIntVal());
	gpDataRepository->addEntry(DataKeyEnum::HEIGHT_IN_CELLS, gpDataRepository->getEntry(DataKeyEnum::HEIGHT_IN_CELLS_LARGE).getUIntVal());
	gpDataRepository->addEntry(DataKeyEnum::NUM_MINES, gpDataRepository->getEntry(DataKeyEnum::NUM_MINES_LARGE).getUIntVal());

}

