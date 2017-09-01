#include "DataLoader.h"
#include "DataRepository.h"
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

DataLoader::DataLoader(const string& filename)
{
	ifstream input( filename );
	if( input.good() )
	{
		while( !input.eof() )
		{
			string line;
			getline( input, line);
			stringstream sstream( line );
			string id;
			sstream >> id;

			if( id == "screen_size" )
			{
				UINT width, height;
				sstream >> width >> height;
				gpDataRepository->addEntry( DataKeyEnum::SCREEN_WIDTH, width );
				gpDataRepository->addEntry( DataKeyEnum::SCREEN_HEIGHT, height );
			}
			else if( id == "cell_size_large" )
			{
				UINT width, height;
				sstream >> width >> height;
				gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS_LARGE, width );
				gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS_LARGE, height );
			}
			else if( id == "num_mines_large" )
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_MINES_LARGE, num );
			}
			else if( id == "cell_size_medium" )
			{
				UINT width, height;
				sstream >> width >> height;
				gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS_MEDIUM, width );
				gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS_MEDIUM, height );
			}
			else if( id == "num_mines_medium" )
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_MINES_MEDIUM, num );
			}
			else if( id == "cell_size_small" )
			{
				UINT width, height;
				sstream >> width >> height;
				gpDataRepository->addEntry( DataKeyEnum::WIDTH_IN_CELLS_SMALL, width );
				gpDataRepository->addEntry( DataKeyEnum::HEIGHT_IN_CELLS_SMALL, height );
			}
			else if( id == "num_mines_small" )
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_MINES_SMALL, num );
			}
			else if( id == "num_games_LMS" )
			{
				UINT num;
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_GAMES_LARGE, num );
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_GAMES_MEDIUM, num );
				sstream >> num;
				gpDataRepository->addEntry( DataKeyEnum::NUM_GAMES_SMALL, num );
			}
			else if( id == "dll_filename" )
			{
				string name;
				sstream >> name;
				gpDataRepository->addEntry( DataKeyEnum::DLL_FILENAME, name );
			}
			else if( id == "target_frame_time" )
			{
				double time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::TARGET_FRAME_TIME, time );
			}
			else if( id == "fire_event_pause" )
			{
				UINT time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::FIRE_EVENT_PAUSE, time );
			}
			else if( id == "game_reset_pause" )
			{
				UINT time;
				sstream >> time;
				gpDataRepository->addEntry( DataKeyEnum::GAME_RESET_PAUSE, time );
			}
			else if (id == "max_reward_time")
			{
				UINT time;
				sstream >> time;
				gpDataRepository->addEntry(DataKeyEnum::MAX_REWARD_TIME, time);
			}
		}
	}

}


