#pragma once

class Point
{
public:
	Point() : Point(0,0) {	}

	Point(int x, int y) : X(x), Y(y) {	}

	Point(const Point& point) : X(point.X), Y(point.Y) { }

	~Point() { }

	int X, Y = 0;
};