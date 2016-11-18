#include "ConvexHull.h"

ConvexHull::ConvexHull()
{
}

void ConvexHull::addPoint(Point* point) {
	points.push_back(point);
	if (point->getX() < left_most) {
		left_most = point->getX();
		left = points.back();
	}
	if (point->getX() > right_most) {
		right_most = point->getX();
		right = points.back();
	}

	if (point->getY() > top_most) {
		top_most = point->getY();
		top = points.back();
	}
	if (point->getY() < bottom_most) {
		bottom_most = point->getY();
		bottom = points.back();
	}
}

void ConvexHull::adjustClockWise() {
	std::vector<Point*> p(points);
	points.clear();

	points.push_back(left);
	for (auto& i : p) {
		if (i->getY() > left->getY() && i->getX() <= top->getX())
			points.push_back(i);
	}
	for (auto& i : p) {
		if (i->getY() > left->getY() && i->getX() > top->getX())
			points.push_back(i);
	}
	for (int i = points.size() - 1; i >= 0; --i)
	{
		if (p[i]->getY() < left->getY())
			points.push_back(p[i]);
	}
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
