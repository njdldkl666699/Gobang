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

//音乐
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

// 载入PNG图并去透明部分
void putimagePNG(int picture_x, int picture_y, IMAGE* picture); //x为载入图片的X坐标，y为Y坐标

// using std::string rather than LPCTSTR !!
void drawSetText(const std::string& str, RECT* rect, const int height,
	LPCTSTR font = _T("宋体"), COLORREF color = BLACK, LONG weight = FW_DONTCARE);