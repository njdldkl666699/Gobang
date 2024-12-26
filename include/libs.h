#pragma once
#include<string>
#include<vector>
#include<chrono>
#include<thread>
#include<fstream>

#include<Windows.h>
#include<graphics.h>
#include<conio.h>
//#include<ctime>
#include<cmath>

//����
#include<mmsystem.h>
#pragma comment (lib,"winmm.lib")

#ifdef _DEBUG
#include<iostream>
#endif // _DEBUG

//#undef max(a,b)
//#undef min(a,b)

const int widgetWidth = 610, widgetHeight = 610;
const int blockLength = 30;
const int xBegPos = 35, yBegPos = 35;
const int xNum = (widgetWidth - 2 * xBegPos) / blockLength + 1;
const int yNum = (widgetHeight - 2 * yBegPos) / blockLength + 1;
const int xEndPos = xBegPos + (xNum - 1) * blockLength;
const int yEndPos = yBegPos + (yNum - 1) * blockLength;

// ����PNGͼ��ȥ͸������
void putimagePNG(int picture_x, int picture_y, IMAGE* picture); //xΪ����ͼƬ��X���꣬yΪY����

// using std::string rather than LPCTSTR !!
void drawSetText(const std::string& str, RECT* rect, const int height,
	LPCTSTR font = _T("����"), COLORREF color = BLACK, LONG weight = FW_DONTCARE);