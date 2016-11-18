#pragma once
#include "DivideAndConquer.h"
#include <iostream>
#include <set>

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
@param points the pointer with all the points
@param begin index starting the sub-vector
@param end index finishing the sub-vector
@param view View interface - if defined call its update method (this way we can implement step by step algorithm presentation)
*/
ConvexHull* DaC_step(std::vector<Point*>* points, int begin, int end, I_View* view) {
	//|points| <= 3 brute-force CH(points) - should be O(1)
	if (end - begin <= 3) {
		ConvexHull* ch = new ConvexHull();
		for (int i = begin; i < end; ++i) {
			ch->addPoint((*points)[i]);
		}
		return ch;
	}
	else { //partition point set into two equal sets and get their convex hulls
		int middle = begin + (end - begin) / 2;
		ConvexHull* lch = DaC_step(points, begin, middle, view);
		ConvexHull* rch = DaC_step(points, middle, end, view);


		if (view)
		{
			std::vector<ConvexHull*> huulls;
			huulls.push_back(lch);
			huulls.push_back(rch);
			view->update(&huulls);
			std::cin.get();
		}


		//merge the two convex hulls
		ConvexHull* merged = mergeHulls(lch, rch);
		if (view) {
			std::vector<ConvexHull*> hulls;
			hulls.push_back(merged);
			view->update(&hulls);
			std::cin.get();
		}

		return merged;
	}
}


/**
safely increments an integer returning 0 if a limit was surpassed
@param limit the limit
@param i the integer to be incremented
*/
int safeIncrement(int limit, int i){
	return ((i+1 >= limit) ? 0 : i+1);
}
/**
safely decrements an integer returning limit-1 if 0 was reached 
@param limit the limit
@param i the integer to be decremented 
*/
int safeDecrement(int limit, int i){
	return ((i-1 < 0) ? limit-1 : i-1);
}

/**
iterates over a convex hull checking all the given criteria and fills an ignore list if a point should be ignored
@param ch the conex hull that holds the points, which are compared
@param start_index what it says
@param end_index what it says
@param comparision_p the point that each point in the iteration is compared to
@param ignorelist the pointer to a vector<int> that will hold all the indexes that are to be ignored
@param iteration this will be called after each iteration, modifying the index generically
@param criteria the criteria, that is used to compare all the points with the comparision_p
*/
int fill_ignore_list(ConvexHull* ch, int start_index, int end_index, int comparision_p, std::set<int>* ignorelist, int (*iteration)(int, int), bool (*criteria)(int, int)) {
	Point* p = ch->at(comparision_p);
	int index_of_first_ignored = 0;
	bool first_ignored = true;
	for (int i = start_index; i != end_index;  i = iteration(ch->size(), i)) {
		if (i != start_index && i != end_index && criteria(ch->at(i)->getX(), p->getX())) {
			ignorelist->insert(i);
			if (first_ignored) {
				index_of_first_ignored = i;
				first_ignored = false;
			}
		}
	}
	return index_of_first_ignored;
}


/**
merges two convex hulls, eliminating vertices inside the new convex hull
@param left the pointer to the first convex hull
@param right the pointer to the second convex hull
*/
ConvexHull* mergeHulls(ConvexHull* first, ConvexHull* second) {


	//generic get (...)-most function TODO try to make all auto functions static
	auto most = [](ConvexHull* ch, float (*genericGetter)(Point* p), bool (*constraint)(int, int)) {
		int wanted = 0;
		for (int i = 1; i < ch->size(); ++i) {
			if (constraint(genericGetter(ch->at(i)), genericGetter(ch->at(wanted)))) {
				wanted = i;
			}
		}
		return wanted;
	};
	//comparision functions
	auto higher = [](int a, int b) { return (a > b); };
	auto lower = [](int a, int b) { return (a < b); };
	//generic getter functions
	auto gety = [](Point* p) { return p->getY(); };
	auto getx = [](Point* p) { return p->getX(); };



	//get top most points (indices) of both left and right convex hull
	int top_most_first	= most(first, gety, higher);
	int top_most_second	= most(second, gety, higher);

	//get bottom most points (indices) of both left and right convex hull
	int bottom_most_first	= most(first, gety, lower);
	int bottom_most_second	= most(second, gety, lower);

	//discard all points in between upper and lower tangent
	int right_most_first = most(first, getx, higher);
	int left_most_first = most(first, getx, lower);

	auto comp_greater = [](int a, int b) {return a > b; };
	auto comp_smaller = [](int a, int b) {return a < b; };

	std::set<int> first_should_ignore;
	int first_ignored_last  = fill_ignore_list(first, right_most_first, left_most_first, bottom_most_first, &first_should_ignore, safeIncrement, comp_greater);
	int first_ignored_first = fill_ignore_list(first, right_most_first, left_most_first, top_most_first, &first_should_ignore, safeDecrement, comp_greater);

	int left_most_second = most(second, getx, lower);
	int right_most_second = most(second, getx, higher);

	std::set<int> second_should_ignore;
	int second_ignored_last  = fill_ignore_list(second, left_most_second, right_most_second, bottom_most_second, &second_should_ignore, safeDecrement, comp_smaller);
	int second_ignored_first = fill_ignore_list(second, left_most_second, right_most_second, top_most_second, &second_should_ignore, safeIncrement, comp_smaller);

	//create a new convex hull using the ignore lists and the first and second convex hulls

	//the new convex hull to return
	ConvexHull* newHull = new ConvexHull();

	/*filling order should be:
	- left_most_first until top_most_first
	- from top_most_second until right_most_second
	- right_most_second until ignore
	- from ignore(last)_first until left_most_first*/

	for (int i = bottom_most_first; i != top_most_first; i = safeIncrement(first->size(), i)) {
		newHull->addPoint(first->at(i));
	}
	newHull->addPoint(first->at(top_most_first));

	for (int i = top_most_second; i != bottom_most_second; i = safeIncrement(second->size(), i)) {
		newHull->addPoint(second->at(i));
	}
	newHull->addPoint(second->at(bottom_most_second));

	//return the new convex hull
	return newHull;
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
