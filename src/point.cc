#include "point.h"

Point DIR[4] = {
	Point(-1,0),
	Point(0,-1),
	Point(1,0),
	Point(0,1)
};

Point operator+(const Point& a, const Point& b)
{
	return Point(a.i+b.i, a.j+b.j);
}

Point operator-(const Point& a, const Point& b)
{
	return Point(a.i-b.i, a.j-b.j);
}

bool operator<(const Point& a, const Point& b)
{
	if (a.i != b.i) return (a.i < b.i);
	else return (a.j < b.j);
}

bool operator==(const Point& a, const Point& b)
{
	return (a.i == b.i && a.j == b.j);
}

bool operator!=(const Point& a, const Point& b)
{
	return !(a == b);
}

int Point::distance(const Point& b) const
{
	return ((i-b.i)*(i-b.i)+(j-b.j)*(j-b.j));
}

