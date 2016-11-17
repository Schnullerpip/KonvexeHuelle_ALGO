#pragma once
#include "DivideAndConquer.h"
#include <iostream>

void swap(Point* a, Point* b);
int partition(std::vector<Point*>* points, int left, int right);
void quick_sort(std::vector<Point*>* points, int left, int right);
ConvexHull* mergeHulls(ConvexHull* left, ConvexHull* right);

void DaC_preparation(std::vector<Point*>* points) {
	//First bring all vertices in order (x scale)
	quick_sort(points, 0, points->size()-1);
}

/**
DaC_recursive processes the recursive steps in the Divide and Conquer Algorithm
*/
ConvexHull* DaC_step(std::vector<Point*>* points, int begin, int end, I_View* view) {
	//|points| <= 3 brute-force CH(points) - should be O(1)
	if (points->size() <= 3 && end - begin <= 3) {
		ConvexHull* ch = new ConvexHull();
		for (int i = begin; i < end; ++i) {
			ch->addPoint((*points)[i]);
		}
		return ch;
	}
	else { //partition point set into two equal sets and get their convex hulls
		int middle = (end - begin) / 2;
		ConvexHull* lch = DaC_step(points, begin, middle, view);
		ConvexHull* rch = DaC_step(points, middle, end, view);

		//merge the two convex hulls
		ConvexHull* merged =  mergeHulls(lch, rch);

		std::vector<ConvexHull*> hulls;
		hulls.push_back(merged);
		view->update(&hulls);

		return merged;
	}
}

/**
merges two convex hulls, eliminating vertices inside the new convex hull
@param left the pointer to the first convex hull
@param right the pointer to the second convex hull
*/
ConvexHull* mergeHulls(ConvexHull* first, ConvexHull* second) {


	//generic get (...)-most function
	auto most = [](ConvexHull* ch, float (*genericGetter)(Point* p), bool (*constraint)(int, int)) {
		Point* wanted = nullptr;
		for (int i = 0; i < ch->size(); ++i) {
			if (!wanted || constraint(genericGetter(ch->at(i)), genericGetter(wanted))) {
				wanted = ch->at(i);
			}
		}
		return wanted;
	};

	//comparision functions
	auto higher = [](int a, int b) {
		return (a > b);
	};
	auto lower = [](int a, int b) {
		return (a < b);
	};

	//generic getter functions
	auto gety = [](Point* p) {
		return p->getY();
	};
	auto getx = [](Point* p) {
		return p->getX();
	};

	//get top most points of both left and right convex hull
	Point* top_most_first	= most(first, gety, higher);
	Point* top_most_second	= most(second, gety, higher);

	//get bottom most points of both left and right convex hull
	Point* bottom_mostfirst		= most(first, gety, lower);
	Point* bottom_most_second	= most(second, gety, lower);

	//get upper tangent
	//get lower tangent

	//the new convex hull to return
	ConvexHull* newHull = new ConvexHull();

	//discard all points in between upper and lower tangent

	//connect the top most points and the bottom_most points

	//return the convex hull
	std::cout << "[DivideAndConquer]::mergeHulls -> ATTENTION! always returns nullptr right now - implement this mock!" << std::endl;
	return nullptr;
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
