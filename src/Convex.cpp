#include "DataInput.h"
#include "Point.h"
#include "ConvexGUI.h"
#include "DivideAndConquer.h"

#include <vector>
#include <iostream>


//Function prototypes
void performanceOptimizedLoop(std::vector<Point*>* points);
void graphicalLoop(std::vector<Point*>* points);
void processArguments(int argc, char** argv, std::vector<Point*>* points);

//Functionpointer, that will execute the 'MODE', that is either graphical or performance optimized - the default is graphical
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
		if (s == "--DataRandom") {//Command line argument for generating Random Vertices
			if (!(argc - i >= 3)) {
				std::cout << "[Convex]::processArguments -> --DataRandom argument needs additional 3 arguments for amount, upper bound & lower bound!" << std::endl;
				continue;
			}
			int amount = atoi(argv[++i]);
			int upper_bound = atoi(argv[++i]);
			int lower_bound = atoi(argv[++i]);
			getPointsFromRandom(points, amount, upper_bound, lower_bound);
		}
		else if (s == "--DataFile") { //Command line argument for reading vertices out of file
			if (!(argc - i >= 1)) {
				std::cout << "[Convex]::processArguments -> --DataFile argument needs additional 1 arguments for the file location!" << std::endl;
			}
			getPointsFromFile(argv[++i], points);
		}
		else if (s == "--performance") { //Command line argument to trigger performance loop mode - default is graphic
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
	DaC_preparation(points);
	while (!view->shouldClose()) {
		DaC_step(points, 0, points->size(), view);
		//view->update();
	}
	delete view;
}