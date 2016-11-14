#pragma once
class Point
{
	float x, y;
public:
	Point(float, float);
	~Point();

	float getX() const;
	float getY() const;

	void setX(float);
	void setY(float);
};

