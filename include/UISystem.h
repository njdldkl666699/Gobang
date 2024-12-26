#pragma once
#include"Timer.h"
#include"ChessBoard.h"

class UISystem
{
public:
	UISystem();

	~UISystem();

private:
	bool gameStart();

	void gamePlay();

	void gameOver();

	void refresh();

	void drawSystem();

private:
	Timer timer;
	ChessBoard cB;
};
