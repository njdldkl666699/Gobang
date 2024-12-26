#include "hardAI.h"

#ifdef _DEBUG
#include<iomanip>
#endif // _DEBUG

using namespace std;
using ChessColor = Chess::ChessColor;

hardAI::hardAI()
{
	// 初始化棋盘分数
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			scoreMap[i][j] = 0;
		}
	}

	// 初始化棋盘信息
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
	// 计分棋盘归0
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			scoreMap[i][j] = 0;
		}
	}

	// 将棋盘信息清空
	for (int i = 0; i < xNum; i++)
	{
		for (int j = 0; j < yNum; j++)
		{
			boardMap[i][j] = ChessColor::Null;
		}
	}

	// 将棋盘上的棋子信息存入boardMap
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

//四个方向
const int hardAI::directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };

////AI分权
//const int hardAI::AIScrWt[4][3] =
//{
//	{0,10,50},{0,300,400},{0,600,3000},{0,30000,30000}
//};
//
////Player分权
//const int hardAI::plrScrWt[4][3] =
//{
//	{0,5,25},{0,250,500},{0,550,2000},{0,20000,20000}
//};

//AI分权
const int hardAI::AIScrWt[4][3] =
{
	{1,3,12},{1,50,700},{1,900,99999},{1,10000000,10000000}
};

//Player分权
const int hardAI::plrScrWt[4][3] =
{
	{1,3,10},{1,40,800},{1,1000,10000},{1,100000,100000}
};

////AI分权
//const int hardAI::AIScrWt[4][3] =
//{
//	{0,10,100},{0,1000,10000},{0,100000,1000000},{0,10000000,10000000}
//};
//
////Player分权
//const int hardAI::plrScrWt[4][3] =
//{
//	{0,10,100},{0,1000,10000},{0,100000,1000000},{0,10000000,10000000}
//};

int hardAI::score(int row, int col, ChessColor clr)
{
	//非空位跳过
	//if (boardMap[row][col] != clr)
	//	return 0;

	//初始化计分板
	//aiScore[row][col] = min(min(row, col), min(xNum - 1 - row, yNum - 1 - col));
	//playerScore[row][col] = min(min(row, col), min(xNum - 1 - row, yNum - 1 - col));

	//当前色的反色
	ChessColor difClr = static_cast<ChessColor>(1 - clr);
	//当前分数
	int score = scoreMap[row][col];
	//检查棋型
	const auto& curScrWt = (clr == nxtClr) ? AIScrWt : plrScrWt;

	//遍历四个方向
	for (int index = 0; index < 4; ++index)
	{
		//当前计数器
		int curCnt = 0;
		//异色计数器
		int difCnt = 0;
		//空位计数器
		int nulCnt = 0;

		//遍历正向和反向
		for (int sgn = 0; sgn <= 1; ++sgn)
		{
			int xDir = directions[index][0];
			int yDir = directions[index][1];
			//当前方向
			xDir = sgn ? xDir : -xDir;
			yDir = sgn ? yDir : -yDir;
			//单向遍历4个子
			for (int i = 0; i <= 4; ++i)
			{
				int x = row + xDir * i;
				int y = col + yDir * i;
				//边界检查
				if (x < 0 || x >= xNum || y < 0 || y >= yNum)
				{
					break;
				}
				//空位检查
				else if (boardMap[x][y] == ChessColor::Null)
				{
					++nulCnt;
					break;
				}
				//异色检查
				else if (boardMap[x][y] == difClr)
				{
					++difCnt;
					break;
				}
				//同色检查
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
		//计分
		if (curCnt != 0)
		{
			score += curScrWt[curCnt - 1][nulCnt];
		}
	}
	// 返回总分
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