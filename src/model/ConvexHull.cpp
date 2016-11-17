#include "ConvexHull.h"

ConvexHull::ConvexHull()
{
}

void ConvexHull::addPoint(Point* point) {
	points.push_back(point);
}

int ConvexHull::size() const
{
	return points.size();
}

Point * ConvexHull::at(int index) const
{
	if(index < points.size())
		return points[index];
	return nullptr;
}


ConvexHull::~ConvexHull()
{
}
