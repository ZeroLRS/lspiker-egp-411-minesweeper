#pragma once

#include <string>
#include "Trackable.h"

class Player:public Trackable
{
public:
	Player(const std::string& name, bool fHuman = false);
	virtual ~Player(){};

	inline const std::string& getName() const { return mName; };

	virtual void update() = 0;
	virtual void setActive( bool isActive ) = 0;
	bool isHuman() const { return mHuman; };
protected:
	std::string mName;
	bool mHuman;
};

