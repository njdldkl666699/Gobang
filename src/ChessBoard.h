#pragma once
#include"libs.h"
#include"Chess.h"
#include"hardAI.h"
#include"easyAI.h"

class ChessBoard
{
	//friend class UISystem;
public:
	enum AIState
	{
		noAI, // multiplayer
		easy, // teacher provided
		hard  // network provided
	};

	enum PlayState
	{
		null,
		move,
		down,
		regret,
		end,
	};

public:
	ChessBoard();

	~ChessBoard();

	//return whether exit
	bool start();

	void checkPressBlock(const RECT& buttonRct);

	void draw() const;

	PlayState getPlayState() const;

	void setPlayState(PlayState playState);

	Chess::ChessColor getPreClr() const;

	PlayState operate();

	void aiOperate();

	void check();

private:
	PlayState playState = null;
	AIState aiState = noAI;
	AI* ai = nullptr;

	Chess previewChess{ 0,0,preClr };
	Chess::ChessColor preClr = Chess::Black;
	std::vector<Chess>chessBoard;
	
	ExMessage mouseEvent{};
	ExMessage mouse{};
	//std::vector<ExMessage> LButtonDownQueue;
	std::thread mouseEventThread;
	bool isMsEvtTrdEnd = false;
};