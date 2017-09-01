#include "StatTracker.h"
#include <fstream>
#include <cassert>

using namespace std;

StatTracker* gpStatTracker = NULL;

StatTracker::StatTracker( const std::string& filename )
	:mFilename( filename )
{
	mStream.open(filename);
	resetAllStats();
}

StatTracker::~StatTracker()
{
	//reportStats();
	mStream.close();
}

void StatTracker::addToStat(StatEnum::StatCategory category, UINT addition)
{
	UINT val = mMap[ category];
	val += addition;
	mMap[ category ] = val;
}

void StatTracker::resetStat(StatEnum::StatCategory category)
{
	mMap[ category ] = 0;
}

void StatTracker::reportStats(std::ostream& theStream) const
{
	
	/*for (UINT i = 0; i<StatEnum::NUM_STAT_CATEGORIES; i++)
	{
		StatEnum::StatCategory category = (StatEnum::StatCategory)i;
		theStream << STAT_DESCRIPTIONS[i] << " = " << mMap.find(category)->second << endl;
	}*/
	for (UINT i = 0; i<StatEnum::NUM_STAT_CATEGORIES; i++)
	{
		StatEnum::StatCategory category = (StatEnum::StatCategory)i;
		theStream << mMap.find(category)->second << ",";
	}
	theStream << endl;
}

void StatTracker::reportStats()
{
	if (mStream.good())
	{
		reportStats(mStream);
	}
}

void StatTracker::reportStatsHeaders()
{
	if (mStream.good())
	{
		mStream << "Name,";

		for (UINT i = 0; i<StatEnum::NUM_STAT_CATEGORIES; i++)
		{
			StatEnum::StatCategory category = (StatEnum::StatCategory)i;
			mStream << STAT_DESCRIPTIONS[i] << ",";
		}
		mStream << endl;
	}
}

void StatTracker::resetAllStats()
{
	mMap.clear();
	for (UINT i = 0; i < StatEnum::NUM_STAT_CATEGORIES; i++)
	{
		mMap[(StatEnum::StatCategory)i] = 0;
	}
}

void StatTracker::logString(const std::string& message)
{
	assert(mStream.good());
	mStream << message << ",";
}

