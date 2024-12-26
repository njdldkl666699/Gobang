#pragma once
#include "AI.h"

class easyAI : public AI
{
	using ChessColor = Chess::ChessColor;
public:
	easyAI();

	//AI下棋接口
	virtual const Chess play(const std::vector<Chess>& chessBoard,
		const ChessColor& preClr)override;

protected:
	//刷新棋盘
	virtual void refresh(const std::vector<Chess>& chessBoard)override;

	void calScore();

	//获取下一步棋子
	virtual const Chess getNextStep()override;

private:
	int playerScore[xNum][yNum]{};
	int aiScore[xNum][yNum]{};
	int scoreMap[xNum][yNum]{};

	//用于存储 chessBoard from AI
	//Chess::ChessColor boardMap[xNum][yNum];

	//下一步棋子颜色
	//ChessColor nxtClr;
private:
	//四个方向
	static const int directions[4][2];
	//AI分权
	static const int AIScrWt[4][3];
	//Player分权
	static const int plrScrWt[4][3];
};