#pragma once
#include <vector>
#include "Point.h"

void getPointsFromFile(const char* path, std::vector<Point*>* out_points);

void getPointsFromRandom(std::vector<Point*>* out_points, int amount, int upper_bound, int lower_bound = 0);

void DrawCirclePoints(int points, double radius, Point center, std::vector<Point*>* out_points);

void DrawSquareAround(std::vector<Point*>* out_points, int amount, int upper_bound, int lower_bound = 0);


