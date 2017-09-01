#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "..\GameLib\EventSystem.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\MouseEvent.h"
#include "..\GameLib\GameState.h"
#include "Game.h"

#include <cassert>
#include <iostream>

using namespace std;

InputSystem::InputSystem()
//:mpEventQueue( NULL )
{
}

void InputSystem::init()
{
	/*if( mpEventQueue == NULL )//not initted yet
	{
	mpEventQueue = al_create_event_queue();
	assert( mpEventQueue != NULL );
	assert( GraphicsSystem::getGraphicsSystem() != NULL );

	al_register_event_source(mpEventQueue, al_get_display_event_source( GraphicsSystem::getGraphicsSystem()->mpDisplay ) );
	al_register_event_source(mpEventQueue, al_get_mouse_event_source());
	al_register_event_source(mpEventQueue, al_get_keyboard_event_source());
	}*/
}

void InputSystem::cleanup()
{
	/*if( mpEventQueue != NULL )//has been initted
	{
	al_unregister_event_source( mpEventQueue, al_get_display_event_source( GraphicsSystem::getGraphicsSystem()->mpDisplay ) );
	al_unregister_event_source(mpEventQueue, al_get_mouse_event_source());
	al_unregister_event_source(mpEventQueue, al_get_keyboard_event_source());
	al_destroy_event_queue( mpEventQueue );
	mpEventQueue = NULL;
	}*/
}

void InputSystem::update()
{
	/*while( !al_is_event_queue_empty( mpEventQueue ) )
	{
	ALLEGRO_EVENT alEvent;
	if ( al_get_next_event( mpEventQueue, &alEvent ) )
	{
	switch( alEvent.type )
	{
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
	EventSystem::getEventSystem()->fireEvent( Event(GAME_QUIT_EVENT, UINT_MAX) );
	break;
	case ALLEGRO_EVENT_KEY_DOWN:
	if( alEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
	{
	EventSystem::getEventSystem()->fireEvent( Event(GAME_QUIT_EVENT, UINT_MAX) );
	}
	break;
	case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
	{
	MouseEvent theEvent( alEvent.mouse.x, alEvent.mouse.y, (MouseButton)alEvent.mouse.button, UINT_MAX );
	EventSystem::getEventSystem()->fireEvent( theEvent );
	break;
	}

	}
	}
	}*/
}
