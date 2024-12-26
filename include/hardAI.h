#pragma once
#include"AI.h"

class hardAI : public AI
{
	using ChessColor = Chess::ChessColor;
public:
	hardAI();

	virtual const Chess play(const std::vector<Chess>& chessBoard,
		const Chess::ChessColor& preClr)override;

private:
	virtual void refresh(const std::vector<Chess>& chessBoard)override;

	//Point MaxScore();
	virtual const Chess getNextStep()override;

	int score(int row, int col, ChessColor clr);

	int eval(int row, int col);

	int minVal(int row, int col, int depth);

	int maxVal(int row, int col, int depth);

private:
	int scoreMap[xNum][yNum];

	//nxtClr�ķ�ɫ
	ChessColor revClr = static_cast<ChessColor>(1 - nxtClr);

	//���ڴ洢 chessBoard from AI
	//Chess::ChessColor boardMap[xNum][yNum];

	//��һ��������ɫ
	//ChessColor nxtClr;

private:
	//�ĸ�����
	static const int directions[4][2];

	//AI��Ȩ
	static const int AIScrWt[4][3];

	//Player��Ȩ
	static const int plrScrWt[4][3];

	// �ж����
	static const int depth = 2;
};