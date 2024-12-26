#include"Chess.h"

void Chess::draw()const
{
	if (color == Null)
		return;
	const int xDst = xBegPos + blockLength * this->x;
	const int yDst = yBegPos + blockLength * this->y;
	const int chessRadius = blockLength * 2 / 5;
	const int chessLength = chessRadius * 2;
	// Í¼Æ¬½Ó¿Ú
	const char* path="";
	if (color == Black)
		path = ".\\res\\black.png";
	else if (color == White)
		path = ".\\res\\white.png";
	std::ifstream ifs(path);
	if (ifs.is_open())
	{
		IMAGE img(chessLength, chessLength);
		TCHAR tPath[16];
		MultiByteToWideChar(CP_ACP, 0, path, -1, tPath, 16);
		loadimage(&img, tPath, chessLength, chessLength);
		putimagePNG(xDst - chessRadius, yDst - chessRadius, &img);
	}
	else
	{
		COLORREF clrref = (color == Black) ? BLACK : WHITE;
		setfillcolor(clrref);
		solidcircle(xDst, yDst, chessRadius);
	}
	ifs.close();
}

bool Chess::operator==(const Chess& other)const
{
	return (this->x == other.x && this->y == other.y 
		&& this->color == other.color);
}

const Chess::ChessColor& Chess::getColor()const
{
	return this->color;
}