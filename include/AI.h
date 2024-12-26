#pragma once
#include"libs.h"
#include"Chess.h"

//�������AI����
class AI
{
	using ChessColor = Chess::ChessColor;
public:
	AI() = default;

	~AI() = default;

	//AI����ӿ�
	virtual const Chess play(const std::vector<Chess>& chessBoard,
		const ChessColor& preClr) = 0;
	
protected:
	//ˢ������
	virtual void refresh(const std::vector<Chess>& chessBoard) = 0;

	//��ȡ��һ������
	virtual const Chess getNextStep() = 0;

protected:
	//���ڴ洢 chessBoard
	ChessColor boardMap[xNum][yNum]{ ChessColor::Null };

	//nxtClr = preClr;
	ChessColor nxtClr{ ChessColor::Null };
};

