#pragma once
#include <vector>

#include "Point.h"

class ConvexHull
{
	std::vector<Point*> points;
public:
	ConvexHull();
	~ConvexHull();

	void addPoint(Point*);
	int size() const;
	Point* at(int index) const;
};

