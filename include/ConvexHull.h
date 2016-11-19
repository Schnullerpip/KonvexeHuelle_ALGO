#pragma once
#include <vector>

#include "Point.h"

class ConvexHull
{
	std::vector<Point*> points;
	int left_most = 1000000;
	Point* left = nullptr;
	int top_most = 0;
	Point* top = nullptr;
	int right_most = 0;
	Point* right = nullptr;
	int bottom_most = 1000000;
	Point* bottom = nullptr;
public:
	ConvexHull();
	~ConvexHull();

	void addPoint(Point*);
	int size() const;
	Point* at(int index) const;

	void adjustClockWise();
};

