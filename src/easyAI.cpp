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

//四个方向
const int easyAI::directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };

//AI分权
const int easyAI::AIScrWt[4][3] =
{
	{0,5,10},{0,25,50},{0,55,300},{0,30000,30000}
};

//Player分权
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
			//非空位跳过
			if (boardMap[row][col] != ChessColor::Null)
				continue;
			//遍历玩家和AI两种情况, 0: 玩家, 1: AI
			for (int stat = 0; stat <= 1; ++stat)
			{
				//遍历四个方向
				for (int index = 0; index < 4; ++index)
				{
					int plrCnt = 0;
					int aiCnt = 0;
					int nullCnt = 0;

					//nxtClr的反色
					auto revClr = static_cast<ChessColor>(1 - nxtClr);
					//当前色
					auto& curClr = stat ? nxtClr : revClr;
					//异色
					auto& difClr = stat ? revClr : nxtClr;
					//同色计数器
					int& curCount = stat ? aiCnt : plrCnt;
					//异色计数器
					int& difCount = stat ? plrCnt : aiCnt;

					//遍历正向和反向
					for (int sgn = 0; sgn <= 1; ++sgn)
					{
						int xDir = directions[index][0];
						int yDir = directions[index][1];
						//当前方向
						xDir = sgn ? xDir : -xDir;
						yDir = sgn ? yDir : -yDir;
						//单向遍历4个子
						for (int i = 1; i < 5; ++i)
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
								++nullCnt;
								break;
							}
							//异色检查
							else if (boardMap[x][y] == difClr)
							{
								++difCount;
								break;
							}
							//同色检查
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

					//当前计分板
					auto& curScore = stat ? aiScore : playerScore;
					//检查棋型（方法2）
					auto& curScrWt = stat ? AIScrWt : plrScrWt;
					//计分
					if (curCount != 0)
						curScore[row][col] += curScrWt[curCount - 1][nullCnt];
				}
			}
			//总分数
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
			//打印行列
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


	//随机选择一个最高分数的点
	int index = rand() % maxPoints.size();
	return Chess(maxPoints[index], nxtClr);
}