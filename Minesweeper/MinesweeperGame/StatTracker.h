#pragma once

#include "..\gamelib\trackable.h"
#include <string>
#include <map>
#include <windows.h>
#include <fstream>

//class only exists to hold a class level enum
class StatEnum
{
public:
	enum StatCategory
	{
		GAMES_SIMULATED_SMALL,
		GAMES_SIMULATED_MEDIUM,
		GAMES_SIMULATED_LARGE,
		GAMES_WON_SMALL,
		GAMES_WON_MEDIUM,
		GAMES_WON_LARGE,
		GAMES_LOST_SMALL,
		GAMES_LOST_MEDIUM,
		GAMES_LOST_LARGE,
		TOTAL_SCORE_SMALL,
		TOTAL_SCORE_MEDIUM,
		TOTAL_SCORE_LARGE, 
		TOTAL_SCORE_OVERALL,
		TOTAL_TIME_SMALL,
		TOTAL_TIME_MEDIUM,
		TOTAL_TIME_LARGE,
		TOTAL_TIME_OVERALL,
		NUM_STAT_CATEGORIES
	};

};

const std::string STAT_DESCRIPTIONS[StatEnum::NUM_STAT_CATEGORIES] = 
{
	"Number of small games Simulated",
	"Number of medium games Simulated",
	"Number of large games Simulated",
	"Number of small games won",
	"Number of medium games won",
	"Number of large games won",
	"Number of small games lost",
	"Number of medium games lost",
	"Number of large games lost",
	"Total score of small games",
	"Total score of medium games",
	"Total score of large games",
	"Total score",
	"Total time to play small games",
	"Total time to play medium games",
	"Total time to play large games",
	"Total time to play"
};



class StatTracker :	public Trackable
{
public:
	StatTracker( const std::string& filename );
	~StatTracker();
	void addToStat( StatEnum::StatCategory category, UINT addition );
	void resetStat( StatEnum::StatCategory category);
	void resetAllStats();
	void reportStats( std::ostream& theStream ) const ;
	void reportStats();
	void reportStatsHeaders();
	void logString(const std::string& message);

private:
	std::string mFilename;
	std::ofstream mStream;
	std::map<StatEnum::StatCategory,UINT> mMap;
};

extern StatTracker* gpStatTracker;