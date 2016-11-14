#pragma once
#include <vector>
#include <Point.h>

class I_View {
protected:
	std::vector<Point*>* points;
public:
	virtual void update(void* credentials = nullptr) = 0;
	virtual bool shouldClose() = 0;
};
