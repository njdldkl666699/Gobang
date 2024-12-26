#pragma once

class Point
{
public:
	Point() :x(0), y(0) {};

	Point(int xPos, int yPos) :x(xPos), y(yPos) {};

	bool operator==(const Point& other)const;

	bool operator!=(const Point& other)const;

	Point operator+(const Point& other)const;

	Point& operator+=(const Point & other);

	int x;
	int y;
};

