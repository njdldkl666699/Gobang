#include "hardAI.h"

#ifdef _DEBUG
#include<iomanip>
#endif // _DEBUG

using namespace std;
using ChessColor = Chess::ChessColor;

hardAI::hardAI()
{
	// ��ʼ�����̷���
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			scoreMap[i][j] = 0;
		}
	}

	// ��ʼ��������Ϣ
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			boardMap[i][j] = ChessColor::Null;
		}
	}
}

const Chess hardAI::play(const std::vector<Chess>& chessBoard,
	const Chess::ChessColor& preClr)
{
	nxtClr = preClr;
	revClr = static_cast<ChessColor>(1 - nxtClr);
	refresh(chessBoard);
	return getNextStep();
}

void hardAI::refresh(const vector<Chess>& chessBoard)
{
	// �Ʒ����̹�0
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			scoreMap[i][j] = 0;
		}
	}

	// ��������Ϣ���
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			boardMap[i][j] = ChessColor::Null;
		}
	}

	// �������ϵ�������Ϣ����boardMap
	for (auto& chess : chessBoard)
	{
		boardMap[chess.x][chess.y] = chess.getColor();
	}
}

const Chess hardAI::getNextStep()
{
	int max = INT_MIN;
#ifdef _DEBUG
	int tempMap[xNum][yNum] = { 0 };
#endif // _DEBUG

	vector<Point> maxscore_pos;
	for (int row = 0; row < xNum; ++row)
	{
		for (int col = 0; col < yNum; ++col)
		{
			if (boardMap[row][col] != ChessColor::Null)
			{
				continue;
			}
			boardMap[row][col] = nxtClr;
			int temp = minVal(row, col, depth - 1);
			temp += min(min(row, col), min(xNum - 1 - row, yNum - 1 - col));

#ifdef _DEBUG
			tempMap[row][col] = temp;
#endif // _DEBUG
			if (temp > max)
			{
				max = temp;
				maxscore_pos.clear();
				maxscore_pos.push_back(Point(row, col));
			}
			else if (temp == max)
			{
				maxscore_pos.push_back(Point(row, col));
			}
			boardMap[row][col] = ChessColor::Null;
		}
	}

#ifdef _DEBUG
	for (int i = 0; i < xNum; ++i)
	{
		for (int j = 0; j < yNum; ++j)
		{
			cout << tempMap[j][i] << setw(4);
		}
		cout << endl;
	}
	cout << endl;
	bool maxscoreMap[xNum][yNum] = { false };
	for (auto& pos : maxscore_pos)
	{
		maxscoreMap[pos.x][pos.y] = true;
	}
	for (int i = 0; i < xNum; ++i)
	{
		for (int j = 0; j < yNum; ++j)
		{
			cout << maxscoreMap[j][i] << setw(4);
		}
		cout << endl;
	}
	cout << endl;
#endif // _DEBUG

	int index = rand() % maxscore_pos.size();
	//return maxscore_pos[index]
	return Chess(maxscore_pos[index], nxtClr);
}

//�ĸ�����
const int hardAI::directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };

////AI��Ȩ
//const int hardAI::AIScrWt[4][3] =
//{
//	{0,10,50},{0,300,400},{0,600,3000},{0,30000,30000}
//};
//
////Player��Ȩ
//const int hardAI::plrScrWt[4][3] =
//{
//	{0,5,25},{0,250,500},{0,550,2000},{0,20000,20000}
//};

//AI��Ȩ
const int hardAI::AIScrWt[4][3] =
{
	{1,3,12},{1,50,700},{1,900,99999},{1,10000000,10000000}
};

//Player��Ȩ
const int hardAI::plrScrWt[4][3] =
{
	{1,3,10},{1,40,800},{1,1000,10000},{1,100000,100000}
};

////AI��Ȩ
//const int hardAI::AIScrWt[4][3] =
//{
//	{0,10,100},{0,1000,10000},{0,100000,1000000},{0,10000000,10000000}
//};
//
////Player��Ȩ
//const int hardAI::plrScrWt[4][3] =
//{
//	{0,10,100},{0,1000,10000},{0,100000,1000000},{0,10000000,10000000}
//};

int hardAI::score(int row, int col, ChessColor clr)
{
	//�ǿ�λ����
	//if (boardMap[row][col] != clr)
	//	return 0;

	//��ʼ���Ʒְ�
	//aiScore[row][col] = min(min(row, col), min(xNum - 1 - row, yNum - 1 - col));
	//playerScore[row][col] = min(min(row, col), min(xNum - 1 - row, yNum - 1 - col));

	//��ǰɫ�ķ�ɫ
	ChessColor difClr = static_cast<ChessColor>(1 - clr);
	//��ǰ����
	int score = scoreMap[row][col];
	//�������
	const auto& curScrWt = (clr == nxtClr) ? AIScrWt : plrScrWt;

	//�����ĸ�����
	for (int index = 0; index < 4; ++index)
	{
		//��ǰ������
		int curCnt = 0;
		//��ɫ������
		int difCnt = 0;
		//��λ������
		int nulCnt = 0;

		//��������ͷ���
		for (int sgn = 0; sgn <= 1; ++sgn)
		{
			int xDir = directions[index][0];
			int yDir = directions[index][1];
			//��ǰ����
			xDir = sgn ? xDir : -xDir;
			yDir = sgn ? yDir : -yDir;
			//�������4����
			for (int i = 0; i <= 4; ++i)
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
					++nulCnt;
					break;
				}
				//��ɫ���
				else if (boardMap[x][y] == difClr)
				{
					++difCnt;
					break;
				}
				//ͬɫ���
				else if (boardMap[x][y] == clr)
				{
					++curCnt;
					/*if (count == 5)
						break;*/
				}
				else
				{
					break;
				}
			}
		}
		//�Ʒ�
		if (curCnt != 0)
		{
			score += curScrWt[curCnt - 1][nulCnt];
		}
	}
	// �����ܷ�
	return score;
}

int hardAI::eval(int row, int col)
{
	int scr = score(row, col, nxtClr) - score(row, col, revClr);
	scoreMap[row][col] = scr;
	return scr;
}

int hardAI::minVal(int row, int col, int depth)
{
	int value = -eval(row, col);
	if (depth <= 0)
	{
		return value;
	}
	int min = INT_MAX;
	for (int r = 0; r < xNum; ++r)
	{
		for (int c = 0; c < yNum; ++c)
		{
			if (boardMap[r][c] != ChessColor::Null)
				continue;
			boardMap[r][c] = revClr;
			int temp = maxVal(r, c, depth - 1);
			if (temp < min)
			{
				min = temp;
			}
			boardMap[r][c] = ChessColor::Null;
		}
	}
	return min;
}

int hardAI::maxVal(int row, int col, int depth)
{
	int value = eval(row, col);
	if (depth <= 0)
	{
		return value;
	}
	int max = INT_MIN;
	for (int r = 0; r < xNum; ++r)
	{
		for (int c = 0; c < yNum; ++c)
		{
			if (boardMap[r][c] != ChessColor::Null)
				continue;
			boardMap[r][c] = nxtClr;
			int temp = minVal(r, c, depth - 1);
			if (temp > max)
			{
				max = temp;
			}
			boardMap[r][c] = ChessColor::Null;
		}
	}
	return max;
}