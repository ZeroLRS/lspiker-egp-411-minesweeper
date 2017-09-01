#include "HumanPlayer.h"
#include "Game.h"
#include "..\GameLib\GameState.h"
#include "..\GameLib\CellClickTransaction.h"
#include "..\GameLib\TransactionHandler.h"
#include <iostream>
#include <windows.h>

using namespace std;

HumanPlayer::HumanPlayer( const std::string& name, TransactionHandler* pHandler )
	:Player(name, true)
	,mpHandler(pHandler)
{
}

HumanPlayer::~HumanPlayer()
{
}

void HumanPlayer::update()
{
	//prompt for play
	//cout << "Human turn...";
}
