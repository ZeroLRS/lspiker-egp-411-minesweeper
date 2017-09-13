#include "AIHolder.h"
#include "Game.h"
#include "..\GameLib\EventSystem.h"
#include "..\GameLib\Event.h"
#include <assert.h>
#include <thread>

using namespace std;


AIHolder::AIHolder( const std::string& playerName, const std::string& dllPath )
	:mName(playerName)
	,mActive(false)
{
	mDllHandle = LoadLibrary(TEXT(dllPath.c_str()));
	
	//hardwired for now!
	EventCallbackFunction address = NULL;
	if( mDllHandle != NULL )
	{
		address = (EventCallbackFunction)GetProcAddress(mDllHandle, MAKE_DECISION.c_str());
		if( address != NULL )
		{
			mFunction = address;
		}
	}
	else
	{
		cout << "DLL file:" << dllPath << " not found!\n";
		assert(false);
	}

}

AIHolder::~AIHolder()
{
	EventSystem::getEventSystem()->unregisterCallback(PICK_A_CELL_EVENT, mFunction);
	FreeLibrary(mDllHandle);
}

void AIHolder::setActive( bool flag )
{
	if( flag && !mActive )//activating
	{
		EventSystem::getEventSystem()->registerCallback(PICK_A_CELL_EVENT, mFunction);
		EventSystem::getEventSystem()->registerCallback(GAME_RESET_NOTIFICATION, mFunction);
		EventSystem::getEventSystem()->registerCallback(GAME_RESET_EVENT, mFunction);
		EventSystem::getEventSystem()->registerCallback(CELL_REVEALED_NOTIFICATION, mFunction);
		EventSystem::getEventSystem()->registerCallback(INIT_EVENT, mFunction);
		EventSystem::getEventSystem()->registerCallback(CLEANUP_EVENT, mFunction);
		mActive = true;
	}
	else if( !flag && mActive )//deactivating
	{
		EventSystem::getEventSystem()->unregisterCallback(PICK_A_CELL_EVENT, mFunction);
		EventSystem::getEventSystem()->unregisterCallback(GAME_RESET_NOTIFICATION, mFunction);
		EventSystem::getEventSystem()->unregisterCallback(GAME_RESET_EVENT, mFunction);
		EventSystem::getEventSystem()->unregisterCallback(CELL_REVEALED_NOTIFICATION, mFunction);
		EventSystem::getEventSystem()->unregisterCallback(INIT_EVENT, mFunction);
		EventSystem::getEventSystem()->unregisterCallback(CLEANUP_EVENT, mFunction);
		mActive = true;
	}
}

