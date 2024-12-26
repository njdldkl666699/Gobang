#include"ChessBoard.h"

using namespace std::literals::chrono_literals;
using std::this_thread::sleep_for;
using ChessColor = Chess::ChessColor;

const auto flushSpeed = 8.33ms;

ChessBoard::ChessBoard()
{

}

ChessBoard::~ChessBoard()
{
	isMsEvtTrdEnd = true;
}

bool ChessBoard::start()
{
	mouseEventThread = std::thread([this]()
		{
			while (!isMsEvtTrdEnd)
			{
				mouseEvent = getmessage(EX_MOUSE);
				if (mouseEvent.message == WM_LBUTTONDOWN || mouseEvent.message == WM_RBUTTONDOWN)
					mouse = mouseEvent;
			}
		});
	mouseEventThread.detach();

	chessBoard.clear();
	playState = null;
	preClr = Chess::Black;
	while (true)
	{
		loadimage(NULL, _T(".\\res\\gamestart.png"), widgetWidth, widgetHeight);
		FlushBatchDraw();

		//Exit Button 503,569;610,610
		if (mouse.x > 503 && mouse.y > 569 && mouse.x < 610 && mouse.y < 610
			&& mouse.message == WM_LBUTTONDOWN)
		{
			mouse.message = WM_MOUSEMOVE;
			return true;
		}

		if (mouse.x > 180 && mouse.x < 430)
		{
			//SinglePlay Button 180,180;430,280
			if (mouse.y > 180 && mouse.y < 280 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				loadimage(NULL, L".\\res\\aiSelect.png", widgetWidth, widgetHeight);
				FlushBatchDraw();
				while (true)
				{
					if (mouse.message == WM_LBUTTONDOWN)
					{
						mouse.message = WM_MOUSEMOVE;
						//Easy Button 180,185;430,275
						if (mouse.y > 185 && mouse.y < 275)
						{
							aiState = AIState::easy;
							ai = new easyAI();
							return false;
						}
						//Hard Button 180,325;430,420
						if (mouse.y > 325 && mouse.y < 420)
						{
							aiState = AIState::hard;
							ai = new hardAI();
							return false;
						}
						//Back Button 
						if (mouse.y > 465 && mouse.y < 560)
						{
							mouse.message = WM_MOUSEMOVE;
							break;
						}
					}
					sleep_for(flushSpeed);
				}
			}
			//DoublePlay Button 180,330;430,420
			if (mouse.y > 330 && mouse.y < 420 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				aiState = AIState::noAI;
				break;
			}
			//GameRules Button 180,460;430,560
			if (mouse.y > 460 && mouse.y < 560 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				loadimage(NULL, _T(".\\res\\gamerule.png"), widgetWidth, widgetHeight);
				FlushBatchDraw();
				//Back Button 180,495;430,585
				while (true)
				{
					if (mouse.y > 495 && mouse.y < 585 && mouse.message == WM_LBUTTONDOWN)
					{
						mouse.message = WM_MOUSEMOVE;
						break;
					}
					sleep_for(flushSpeed);
				}
			}
		}
		sleep_for(flushSpeed);
	}
	return false;
}

void ChessBoard::checkPressBlock(const RECT& buttonRct)
{
	while (true)
	{
		if (mouse.x > buttonRct.left && mouse.x < buttonRct.right 
			&& mouse.y > buttonRct.top && mouse.y < buttonRct.bottom 
			&& mouse.message == WM_LBUTTONDOWN)
		{
			mouse.message = WM_MOUSEMOVE;
			break;
		}
		sleep_for(flushSpeed);
	}
}

void ChessBoard::draw() const
{
	previewChess.draw();
	for (const auto& i : chessBoard)
		i.draw();
}

ChessBoard::PlayState ChessBoard::getPlayState() const
{
	return playState;
}

void ChessBoard::setPlayState(PlayState playState)
{
	this->playState = playState;
}

ChessColor ChessBoard::getPreClr() const
{
	return preClr;
}

ChessBoard::PlayState ChessBoard::operate()
{
#ifdef NDEBUG
	std::cout << "x: " << mouse.x << " y: " << mouse.y <<
		" message: " << (mouse.message == WM_LBUTTONDOWN ? "Lbutton" : "") << std::endl;
#endif // NDEBUG
	//悔棋
	if (mouse.message == WM_RBUTTONDOWN && !chessBoard.empty())
	{
		mouse.message = WM_MOUSEMOVE;
		chessBoard.pop_back();
		if (aiState != AIState::noAI)
			chessBoard.pop_back();
		else
			preClr = static_cast<ChessColor>(1 - static_cast<int>(preClr));
		return regret;
	}

	int xPos = (int)round((mouseEvent.x - xBegPos) / static_cast<float>(blockLength));
	int yPos = (int)round((mouseEvent.y - yBegPos) / static_cast<float>(blockLength));
	if (xPos >= 0 && xPos < xNum && yPos >= 0 && yPos < yNum)
	{
		for (const auto& i : chessBoard)
			if (i.x == xPos && i.y == yPos)
				return null;

		previewChess = Chess(xPos, yPos, preClr);
		//putDownChess
		if (mouse.message == WM_LBUTTONDOWN)
		{
			mouse.message = WM_MOUSEMOVE;
			chessBoard.push_back(previewChess);
			preClr = static_cast<ChessColor>(1 - static_cast<int>(preClr));
			return down;
		}
	}

	return move;
}

void ChessBoard::aiOperate()
{
	if (playState == PlayState::end || aiState == AIState::noAI)
		return;
	chessBoard.push_back(ai->play(chessBoard, preClr));
	preClr = static_cast<ChessColor>(1 - static_cast<int>(preClr));
	//check();
	//refresh();
}

void ChessBoard::check()
{
	static const int directions[4][2] = { {1,0},{0,1},{1,1},{1,-1} };
	const auto& curChess = chessBoard.back();
	//四个方向
	for (int i = 0; i < 4; ++i)
	{
		int count = 0;
		for (int sgn = 0; sgn <= 1; ++sgn)
		{
			int xDir = directions[i][0];
			int yDir = directions[i][1];
			xDir = sgn ? xDir : -xDir;
			yDir = sgn ? yDir : -yDir;

			bool isHit = false;
			for (int k = 1; k <= 4 && !isHit; ++k)
			{
				int x = curChess.x + xDir * k;
				int y = curChess.y + yDir * k;
				for (const auto& chess : chessBoard)
				{
					if (chess.x == x && chess.y == y)
					{
						if (chess.getColor() == curChess.getColor())
							count++;
						else
							isHit = true;
						break;
					}
				}
			}
		}
		if (count >= 4)
		{
			playState = end;
			break;
		}
	}
}