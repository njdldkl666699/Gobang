#include"UISystem.h"

using namespace std::literals::chrono_literals;
using std::this_thread::sleep_for;

const auto flushSpeed = 8.33ms;

UISystem::UISystem()
{
#ifdef _DEBUG
	initgraph(widgetWidth, widgetHeight, EX_SHOWCONSOLE);
#else // RELEASE
	initgraph(widgetWidth, widgetHeight);
#endif
	srand((unsigned int)time(nullptr));
	BeginBatchDraw();
	while (true)
	{
		bool isExit = gameStart();
		if (isExit)
			break;
		timer.start();
		gamePlay();
		timer.stop();
		gameOver();
	}
}

UISystem::~UISystem()
{
	EndBatchDraw();
	closegraph();
}

bool UISystem::gameStart()
{
	bool isExit = cB.start();
	if (isExit)
		return isExit;
	mciSendString(_T("open .\\res\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("play bgm repeat"), 0, 0, 0);

	setbkmode(TRANSPARENT);
	setbkcolor(RGB(245, 202, 105));
	setlinecolor(BLACK);
	return isExit;
}

void UISystem::gamePlay()
{
	while (cB.getPlayState() != ChessBoard::end)
	{
		cB.setPlayState(cB.operate());
		switch (cB.getPlayState())
		{
		case ChessBoard::move:
			refresh();
			break;
		case ChessBoard::down:
			cB.check();
			refresh();
			cB.aiOperate();
			cB.check();
			refresh();
			break;
		case ChessBoard::regret:
			refresh();
			break;
		default:
			break;
		}
		sleep_for(flushSpeed);
	}
}

void UISystem::gameOver()
{
	mciSendString(_T("stop bgm"), 0, 0, 0);
	mciSendString(_T("close bgm"), 0, 0, 0);
	//(void)_getch();
	cB.checkPressBlock({ 0,0,widgetWidth,widgetHeight });

	cleardevice();
	loadimage(NULL, _T(".\\res\\gameover.png"), widgetWidth, widgetHeight);

	RECT textRct = { xBegPos, 2 * yBegPos, xEndPos, yEndPos };
	//preClr为黑，表明白子胜，反之同理
	std::string textStr = "游戏结束！\n";
	textStr += (cB.getPreClr() == Chess::ChessColor::Black) ? "白" : "黑";
	textStr += "方获胜\n\n游戏时长\n";
	textStr += std::to_string(static_cast<int>(timer.duration()));
	textStr += " 秒";
	drawSetText(textStr, &textRct, 72, _T("隶书"), BLACK, FW_DONTCARE);

	FlushBatchDraw();
	cB.checkPressBlock({ 180,485,430,580 });
}

void UISystem::refresh()
{
	cleardevice();
	drawSystem();
	cB.draw();
	FlushBatchDraw();
}

void UISystem::drawSystem()
{
	static const int textHeight = 24;
	static const int offset = 10;

	//背景图接口
	std::ifstream ifs(".\\res\\gameplay.png");
	if (ifs.is_open())
	{
		IMAGE img(widgetWidth, widgetHeight);
		loadimage(&img, _T(".\\res\\gameplay.png"), widgetWidth, widgetHeight);
		putimage(0, 0, &img);
	}
	ifs.close();

	//竖线 & num
	for (int i = 0; i < xNum; i++)
	{
		int x = xBegPos + i * blockLength;
		line(x, yBegPos, x, yEndPos);
		RECT numRct = { x - offset, yBegPos - textHeight, x + textHeight - offset, yBegPos };
		drawSetText(std::to_string(i + 1), &numRct, textHeight, L"Comic Sans MS", BLACK, FW_BOLD);
	}

	//横线 & char
	for (int i = 0; i < yNum; i++)
	{
		int y = yBegPos + i * blockLength;
		line(xBegPos, y, xEndPos, y);
		RECT charRct = { xBegPos - textHeight, y - offset, xBegPos, y + textHeight - offset };
		char chr[2] = { 'A' + i , 0 };
		drawSetText(chr, &charRct, textHeight, L"Comic Sans MS", BLACK, FW_BOLD);
	}
}