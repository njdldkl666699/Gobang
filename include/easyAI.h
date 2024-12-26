#pragma once
#include "AI.h"

class easyAI : public AI
{
	using ChessColor = Chess::ChessColor;
public:
	easyAI();

	//AI����ӿ�
	virtual const Chess play(const std::vector<Chess>& chessBoard,
		const ChessColor& preClr)override;

protected:
	//ˢ������
	virtual void refresh(const std::vector<Chess>& chessBoard)override;

	void calScore();

	//��ȡ��һ������
	virtual const Chess getNextStep()override;

private:
	int playerScore[xNum][yNum]{};
	int aiScore[xNum][yNum]{};
	int scoreMap[xNum][yNum]{};

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
};