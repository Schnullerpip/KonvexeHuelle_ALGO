#include "Point.h"
#include <string>
#include <iostream>

Point::Point(float X, float Y)
{
	x = X;
	y = Y;
}

Point::~Point() { }

void Point::setX(float newx) {
	x = newx;
}

void Point::setY(float newy) {
	y = newy;
}

float Point::getX() const {
	return x;
}

float Point::getY() const {
	return y;
} 