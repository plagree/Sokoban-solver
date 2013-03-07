#ifndef __POINT_H__
#define __POINT_H__

#define UP DIR[0]
#define LEFT DIR[1]
#define DOWN DIR[2]
#define RIGHT DIR[3]

class Point
{
	public:

		int distance(const Point& b) const;

		Point(): i(0),j(0) {};
		Point(int i, int j): i(i), j(j) { };
		int i;
		int j;
};

Point operator+(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);
bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);

extern Point DIR[4];

#endif /*__POINT_H */
