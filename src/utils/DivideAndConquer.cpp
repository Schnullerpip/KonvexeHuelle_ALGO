#pragma once
#include "DivideAndConquer.h"
#include "I_View.hpp"
#include <iostream>

void swap(Point* a, Point* b);
int partition(std::vector<Point*>* points, int left, int right);
void quick_sort(std::vector<Point*>* points, int left, int right);

void DaC_preparation(std::vector<Point*>* points) {
	//First bring all vertices in order (x scale)
	quick_sort(points, 0, points->size()-1);
}

/**
DaC_recursive processes the recursive steps in the Divide and Conquer Algorithm
*/
void DaC_step(std::vector<Point*>* points, ConvexHull* ch) {
	//|points| <= 3 brute-force CH(points) - should be O(1)
	if (points->size() <= 3) {
		//TODO the actual algorithm
	}
	//TODO the actual algorithm
}

/**
Help function to swap to indexes
*/
void swap(Point* a, Point* b) {
	Point c = *a;
	*a = *b;
	*b = c;
}

/**Useful function that puts the left most element of a set of ints to its right place in the set
* wont sort the set but everything left of the pivot will be smaller than the pivot*/
int partition(std::vector<Point*>* points, int left,int right)
{
    int pivot= (*points)[left]->getX();
    int i=left;

    for(int j=left+1; j<=right; +j++)
    {
        if((*points)[j]->getX()<=pivot)
        {
            i=i+1;
            swap((*points)[i],(*points)[j]);
        }
    }

	swap((*points)[i],(*points)[left]);
    return i;
}

/**the quickSort algorithm sorting a set of ints considering left and right values*/
void quick_sort(std::vector<Point*>* points, int left, int right)
{
	int r;
	if (left < right)
	{
		r = partition(points, left, right);
		quick_sort(points, left, r-1);
		quick_sort(points, r+1, right);
	}
}
