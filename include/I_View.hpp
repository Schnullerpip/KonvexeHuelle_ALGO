#pragma once
#include <vector>
#include "Point.h"
#include "ConvexHull.h"

class I_View {
protected:
	std::vector<Point*>* points;
public:
	virtual void update(std::vector<ConvexHull*>* ch  = nullptr) = 0;
	virtual bool shouldClose() = 0;
};
