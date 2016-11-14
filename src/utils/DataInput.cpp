#include "DataInput.h"
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>

/**
extrudes points stored in a file and writes them into a vector -> dont forget to properly delete the vector!
@param path path to the file
@param out_points the vector that holds Point pointers and is filled*/
void getPointsFromFile(const char * path, std::vector<Point*>* out_points)
{
	std::ifstream input(path);
	if (!input) {
		std::cout << "error occured while trying to open file: " << path << std::endl;
		return;
	}

	unsigned number_points = 0;
	float x, y;

	while (!input.eof()) {
		if (number_points == 0) {
			input >> number_points;
		}
		else {
			input >> x;
			input >> y;
			out_points->push_back(new Point(x, y));
		}
	}
	input.close();
}

void getPointsFromRandom(std::vector<Point*>* out_points, int amount, int upper_bound, int lower_bound) {

	std::random_device rd;
	std::mt19937 mt(time(NULL));
	mt.discard(10000);
	std::uniform_int_distribution<> dist(lower_bound, upper_bound);

	for (int i = 0; i < amount; ++i) {
		out_points->push_back(new Point(dist(mt), dist(mt)));
	}
}