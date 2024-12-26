#pragma once
#include"libs.h"
#include"Chess.h"

//纯虚抽象AI基类
class AI
{
	using ChessColor = Chess::ChessColor;
public:
	AI() = default;

	~AI() = default;

	//AI下棋接口
	virtual const Chess play(const std::vector<Chess>& chessBoard,
		const ChessColor& preClr) = 0;
	
protected:
	//刷新棋盘
	virtual void refresh(const std::vector<Chess>& chessBoard) = 0;

	//获取下一步棋子
	virtual const Chess getNextStep() = 0;

protected:
	//用于存储 chessBoard
	ChessColor boardMap[xNum][yNum]{ ChessColor::Null };

	//nxtClr = preClr;
	ChessColor nxtClr{ ChessColor::Null };
};

