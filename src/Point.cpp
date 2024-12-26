#include "Point.h"

bool Point::operator==(const Point& other)const
{
	return (this->x == other.x && this->y == other.y);
}

Point Point::operator+(const Point& other)const
{
	Point tmp;
	tmp.x = this->x + other.x;
	tmp.y = this->y + other.y;
	return tmp;
}

Point& Point::operator+=(const Point& other)
{
	*this = *this + other;
	return *this;
}

bool Point::operator!=(const Point& other)const
{
	return !(*this == other);
}