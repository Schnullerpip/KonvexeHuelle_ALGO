#include "DataInput.h"
#include "Point.h"
#include "ConvexGUI.h"

#include <vector>
#include <iostream>


void performanceOptimizedLoop(std::vector<Point*>* points);
void graphicalLoop(std::vector<Point*>* points);
void processArguments(int argc, char** argv, std::vector<Point*>* points);

void(*loop)(std::vector<Point*>* points) = graphicalLoop;

int main(int argc, char** argv) {

	std::vector<Point*> points;

	processArguments(argc, argv, &points);

	loop(&points);

	//delete the points in vector:points
	for (auto p : points) delete p;
	return 0;
}


/**
processes the command line arguments
@param argc the number of arguments
@param argv an array of char* which hold the actual input
@param points the array of Point* which are filled depending on the DataGeneration Method (that is given by the command line arguments)*/
void processArguments(int argc, char** argv, std::vector<Point*>* points) {
	for (int i = 0; i < argc; ++i) {
		std::string s(argv[i]);
		if (s == "--DataRandom") {
			if (!(argc - i >= 3)) {
				std::cout << "[Convex]::processArguments -> --DataRandom argument needs additional 3 arguments for amount, upper bound & lower bound" << std::endl;
				continue;
			}
			int amount = atoi(argv[++i]);
			int upper_bound = atoi(argv[++i]);
			int lower_bound = atoi(argv[++i]);
			getPointsFromRandom(points, amount, upper_bound, lower_bound);
		}
		else if (s == "--DataFile" && argc - i >= 1) {
			getPointsFromFile(argv[++i], points);
		}
		else if (s == "--performance") {
			loop = performanceOptimizedLoop;
		}
	}
}
/**
An application loop, that will jut process the convex algorithm, without graphically displaying it and also measuring the process*/
void performanceOptimizedLoop(std::vector<Point*>* points) {

	//when everything is finished and results are displayed wait for the user to close the application
	std::cin.get();
}

/**
An application loop, that will provide a GUI for the user and also displays the points and the convex frame (step by step)*/
void graphicalLoop(std::vector<Point*>* points) {
	I_View* view = new ConvexGUI(points);
	while (!view->shouldClose()) {
		view->update();
	}
	delete view;
}