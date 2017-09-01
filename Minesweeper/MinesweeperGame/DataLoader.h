#pragma once
#include "..\gamelib\trackable.h"
#include <string>

class DataLoader : public Trackable
{
public:
	DataLoader( const std::string& filename );
	~DataLoader(){};

};

