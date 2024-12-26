#pragma once
#include"libs.h"
#include"Point.h"

class Chess :public Point
{
public:
	enum ChessColor
	{
		Black, White, Null
	};

	Chess() :Point(), color(Null) {};

	Chess(int xPos, int yPos, ChessColor clr)
		:Point(xPos, yPos), color(clr) {};

	Chess(const Chess& other)
		:Chess(other.x, other.y, other.color) {};

	Chess(const Point& p, ChessColor clr)
		:Point(p), color(clr) {};

	void draw()const;

	bool operator==(const Chess& other)const;

	const ChessColor& getColor()const;

private:
	ChessColor color;
};