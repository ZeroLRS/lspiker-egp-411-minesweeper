#include <iostream>
#include "DLLCode.h"
#include "DLLUtil.h"
#include "..\GameLib\GameView.h"
#include "..\GameLib\Transaction.h"
#include "..\GameLib\CellClickTransaction.h"
#include "..\GameLib\DLLInittedTransaction.h"
#include "..\GameLib\TransactionHandler.h"
#include "..\GameLib\Event.h"
#include "..\GameLib\PickCellEvent.h"
#include "..\GameLib\ResetGameEvent.h"
#include <windows.h>
#include "resource.h"
#include "../GameLib/GameResetNotification.h"
#include "../GameLib/CellRevealedNotification.h"

void init(TransactionHandler* pHandler)
{
	pHandler->postTransaction(new DLLInittedTransaction(getMessageFromStringTable()));
	pHandler->postTransaction(new DLLInittedTransaction(getMessageFromFileResource()));
}

__declspec(dllexport) void makeDecision(const Event& theEvent, TransactionHandler* pHandler)
{

	if( theEvent.getType() == PICK_A_CELL_EVENT )
	{
		const PickCellEvent& pickEvent = static_cast<const PickCellEvent&>(theEvent);
		const GameView& theView = pickEvent.getGameView();

		while(true)
		{
			//choose a random cell until we find one that is unrevealed
			UINT index = rand() % theView.getNumCells();
			HiddenCell cell = theView.getCell( index );
			if( !cell.isRevealed() )
			{
				pHandler->postTransaction( new CellClickTransaction(index) );
				break;
			}
		}
	}
	else if (theEvent.getType() == GAME_RESET_NOTIFICATION)//a new game is about to be played
	{
		//respond to a new game about to start
		const GameResetNotification& resetEvent = static_cast<const GameResetNotification&>(theEvent);
		UINT numMines = resetEvent.getNumMines();
		UINT width = resetEvent.getWidth();
		UINT height = resetEvent.getHeight();

		printf("\nNew Map Data\nWidth: %u\nHeight: %u\nNum Mines: %u\n", width, height, numMines);
	}
	else if (theEvent.getType() == INIT_EVENT)//game is being initialized for the first time (about to enter game loop)
	{
		//respond to the game being initialized for this DLL
		init(pHandler);
	}
	else if (theEvent.getType() == CLEANUP_EVENT)//game is being shut down - cleanup any memory
	{
		//respond to the game being cleaned-up for this DLL
	}
	else if (theEvent.getType() == CELL_REVEALED_NOTIFICATION)
	{
		const CellRevealedNotification& cellNot = static_cast<const CellRevealedNotification&>(theEvent);
		printf("%u ", cellNot.getRevealedCell());
	}
	return;
}

string getMessageFromStringTable()
{
	char buffer[1024];
	HINSTANCE hInst = (HINSTANCE)GetCurrentModule();
	int numRead = LoadStringA(hInst, IDS_STRING103, buffer, 1023);
	return string(buffer);
}

string getMessageFromFileResource()
{
	char buffer[1024];
	HINSTANCE hInst = (HINSTANCE)GetCurrentModule();
	HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(IDR_TEXT1), RT_RCDATA);
	HGLOBAL hGlobal = LoadResource(hInst, hRes);
	DWORD dwSize = SizeofResource(hInst, hRes);
	LPVOID ptr = LockResource(hGlobal);
	strncpy_s(buffer, (char*)ptr, dwSize);
	UnlockResource(hGlobal);

	return string(buffer);
}
