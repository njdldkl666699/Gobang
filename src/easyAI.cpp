#include"easyAI.h"
#ifdef _DEBUG
#include<iomanip>
#endif // _DEBUG

using namespace std;

easyAI::easyAI()
{
	for (int i = 0; i < xNum; ++i)
	{
		for (int j = 0; j < yNum; ++j)
		{
			int defaultScore = min(min(i, j), min(xNum - 1 - i, yNum - 1 - j));

#ifdef _DEBUG
			cout << defaultScore << ' ';
			if (j == yNum - 1)
				cout << endl;
#endif // _DEBUG

			playerScore[i][j] = defaultScore;
			aiScore[i][j] = defaultScore;
		}
	}
}

const Chess easyAI::play(const vector<Chess>& chessBoard,
	const ChessColor& preClr)
{
	nxtClr = preClr;
	refresh(chessBoard);
	return getNextStep();
}

void easyAI::refresh(const vector<Chess>& chessBoard)
{
	for (int i = 0; i < xNum; ++i)
	{
		for (int j = 0; j < yNum; ++j)
		{
			int defaultScore = min(min(i, j), min(xNum - 1 - i, yNum - 1 - j));
			playerScore[i][j] = defaultScore;
			aiScore[i][j] = defaultScore;
			scoreMap[i][j] = 0;
			boardMap[i][j] = ChessColor::Null;
		}
	}

	for (const auto& chess : chessBoard)
	{
		boardMap[chess.x][chess.y] = chess.getColor();
	}
}

//�ĸ�����
const int easyAI::directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };

//AI��Ȩ
const int easyAI::AIScrWt[4][3] =
{
	{0,5,10},{0,25,50},{0,55,300},{0,30000,30000}
};

//Player��Ȩ
const int easyAI::plrScrWt[4][3] =
{
	{0,5,10},{0,30,40},{0,60,200},{0,20000,20000}
};

void easyAI::calScore()
{
	for (int row = 0; row < xNum; ++row)
	{
		for (int col = 0; col < yNum; ++col)
		{
			//�ǿ�λ����
			if (boardMap[row][col] != ChessColor::Null)
				continue;
			//������Һ�AI�������, 0: ���, 1: AI
			for (int stat = 0; stat <= 1; ++stat)
			{
				//�����ĸ�����
				for (int index = 0; index < 4; ++index)
				{
					int plrCnt = 0;
					int aiCnt = 0;
					int nullCnt = 0;

					//nxtClr�ķ�ɫ
					auto revClr = static_cast<ChessColor>(1 - nxtClr);
					//��ǰɫ
					auto& curClr = stat ? nxtClr : revClr;
					//��ɫ
					auto& difClr = stat ? revClr : nxtClr;
					//ͬɫ������
					int& curCount = stat ? aiCnt : plrCnt;
					//��ɫ������
					int& difCount = stat ? plrCnt : aiCnt;

					//��������ͷ���
					for (int sgn = 0; sgn <= 1; ++sgn)
					{
						int xDir = directions[index][0];
						int yDir = directions[index][1];
						//��ǰ����
						xDir = sgn ? xDir : -xDir;
						yDir = sgn ? yDir : -yDir;
						//�������4����
						for (int i = 1; i < 5; ++i)
						{
							int x = row + xDir * i;
							int y = col + yDir * i;
							//�߽���
							if (x < 0 || x >= xNum || y < 0 || y >= yNum)
							{
								break;
							}
							//��λ���
							else if (boardMap[x][y] == ChessColor::Null)
							{
								++nullCnt;
								break;
							}
							//��ɫ���
							else if (boardMap[x][y] == difClr)
							{
								++difCount;
								break;
							}
							//ͬɫ���
							else if (boardMap[x][y] == curClr)
							{
								++curCount;
								/*if (count == 5)
									break;*/
							}
							else
							{
								break;
							}
						}
					}

					//��ǰ�Ʒְ�
					auto& curScore = stat ? aiScore : playerScore;
					//������ͣ�����2��
					auto& curScrWt = stat ? AIScrWt : plrScrWt;
					//�Ʒ�
					if (curCount != 0)
						curScore[row][col] += curScrWt[curCount - 1][nullCnt];
				}
			}
			//�ܷ���
			scoreMap[row][col] += aiScore[row][col] + playerScore[row][col];
		}
	}
}

const Chess easyAI::getNextStep()
{
	int maxScore = 0;
	vector<Point> maxPoints;
	calScore();
	for (int i = 0; i < xNum; ++i)
	{
		for (int j = 0; j < yNum; ++j)
		{
#ifdef _DEBUG
			//��ӡ����
			cout << scoreMap[j][i] << setw(4);
			if (j == yNum - 1)
				cout << endl;
#endif // _DEBUG
			if (scoreMap[i][j] > maxScore)
			{
				maxPoints.clear();
				maxScore = scoreMap[i][j];
				maxPoints.emplace_back(i, j);
			}
			else if (scoreMap[i][j] == maxScore)
			{
				maxPoints.emplace_back(i, j);
			}
		}
	}
#ifdef _DEBUG
	cout << endl;
#endif // _DEBUG


	//���ѡ��һ����߷����ĵ�
	int index = rand() % maxPoints.size();
	return Chess(maxPoints[index], nxtClr);
}