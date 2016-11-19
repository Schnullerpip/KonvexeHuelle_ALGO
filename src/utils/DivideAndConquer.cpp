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
	if (end - begin < 3) {
		ConvexHull* ch = new ConvexHull();
		for (int i = begin; i < end; ++i) {
			ch->addPoint((*points)[i]);
		}
		return ch;
	}
	else if (end - begin == 3) {
		ConvexHull* ch = new ConvexHull();
		/*
		make sure to add them clock wise, that simplifies merging a lot
		If we have 3 points we know that idx 0 is left most and idx 2 is right most, because they are sorted by x-coords, 
		for clock wise purpose we want to add the next point according to its y coord -> the higher one comes next
		*/
		bool higher = (*points)[begin + 1]->getY() > (*points)[begin+2]->getY();
		ch->addPoint((*points)[begin]);
		if (higher) {
			ch->addPoint((*points)[begin+1]);
			ch->addPoint((*points)[begin+2]);
		}
		else {
			ch->addPoint((*points)[begin+2]);
			ch->addPoint((*points)[begin+1]);
		}
		return ch;
	}
	else { //partition point set into two equal sets and get their convex hulls
		int middle = begin + (end - begin) / 2;
		ConvexHull* lch = DaC_step(points, begin, middle, view);
		ConvexHull* rch = DaC_step(points, middle, end, view);


		if (view)
		{
			std::vector<ConvexHull*> to_be_merged;
			to_be_merged.push_back(lch);
			to_be_merged.push_back(rch);
			view->update(&to_be_merged);
			view->getInput();
		}

		//merge the two convex hulls
		ConvexHull* merged = mergeHulls(lch, rch);

		delete lch;
		delete rch;

		if (view) {
			std::vector<ConvexHull*> hulls;
			hulls.push_back(merged);
			view->update(&hulls);
			view->getInput();
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
generic get (...)-most function
*/
int most(ConvexHull* ch, float(*genericGetter)(Point* p), bool(*constraint)(int, int)){
	int wanted = 0;
	for (int i = 1; i < ch->size(); ++i) {
		if (constraint(genericGetter(ch->at(i)), genericGetter(ch->at(wanted)))) {
			wanted = i;
		}
	}
	return wanted;
}

/*functions used as arguments for generic use of most function*/
bool higher(int a, int b) { return a >= b; }
bool lower(int a, int b) { return a <= b; }
float getx(Point* p) { return p->getX(); }
float gety(Point* p) { return p->getY(); }
float getxy(Point* p) { return p->getX() + p->getY(); }


float triangleVolume(Point* A, Point* B, Point* C) {
	float volume = 0;
	volume = A->getX()*(B->getY() - C->getY()) + B->getX()*(C->getY() - A->getY()) + C->getX() * (A->getY() - B->getY());
	volume /= 2;
	if (volume < 0)volume *= -1;

	return volume;
}

bool quadrilateralIntersection(Point* p, Point* A, Point* B, Point* C, Point* D, float volume) {
	return (triangleVolume(p, A, B) +
		triangleVolume(p, B, C) +
		triangleVolume(p, C, D) +
		triangleVolume(p, D, A)) == volume;

}

/**
merges two convex hulls, eliminating vertices inside the new convex hull
@param left the pointer to the first convex hull
@param right the pointer to the second convex hull
*/
ConvexHull* mergeHulls(ConvexHull* first, ConvexHull* second) {

	int left_most_second = most(second, getx, lower);
	int right_most_first  = most(first, getx, higher);
	
	int upper_merge_point_first = right_most_first;
	int upper_merge_point_second = left_most_second;

	int bottom_merge_point_first = right_most_first;
	int bottom_merge_point_second = left_most_second;

	/**
	returns >0 if p is left of the line left-right, =0 if p is on line left-right, <0 if p is right of the line left-right
	*/
	auto onTangent = [](Point* A, Point* B, Point* p) {
		return (B->getX() - A->getX())*(p->getY() - A->getY()) - (p->getX() - A->getX())*(B->getY() - A->getY());
	};

	auto isLeft = [onTangent](Point* A, Point* B, Point* p) {
		float value = onTangent(A, B, p);
		return value >= 0;
	};
	auto isRight = [onTangent](Point* A, Point* B, Point* p) {
		float value = onTangent(A, B, p);
		return value <= 0;
	};


	//find the upper merge point
	bool first_merge_point_ready = false;
	bool second_merge_point_ready = false;
	do {
		Point* left  = first->at(safeDecrement(first->size(), upper_merge_point_first));
		Point* ump_f = first->at(upper_merge_point_first);
		Point* ump_s = second->at(upper_merge_point_second);
		Point* right = second->at(safeIncrement(second->size(), upper_merge_point_second));

		//left-ump_f must be a right-turn
		first_merge_point_ready = isLeft(ump_s, ump_f, left);
		//ump_s-right must be a left-turn
		second_merge_point_ready = isRight(ump_f, ump_s, right);

		if (!first_merge_point_ready)
			upper_merge_point_first = safeDecrement(first->size(), upper_merge_point_first);
		if (!second_merge_point_ready)
			upper_merge_point_second = safeIncrement(second->size(), upper_merge_point_second);

	} while (!(first_merge_point_ready && second_merge_point_ready));

	//find the bottom merge point
	first_merge_point_ready = false;
	second_merge_point_ready = false;
	do {
		Point* left  = first->at(safeIncrement(first->size(), bottom_merge_point_first));
		Point* ump_f = first->at(bottom_merge_point_first);
		Point* ump_s = second->at(bottom_merge_point_second);
		Point* right = second->at(safeDecrement(second->size(), bottom_merge_point_second));

		//left-ump_f must be a right-turn
		first_merge_point_ready = isRight(ump_s, ump_f, left);
		//ump_s-right must be a left-turn
		second_merge_point_ready = isLeft(ump_f, ump_s, right);

		if (!first_merge_point_ready)
			bottom_merge_point_first = safeIncrement(first->size(), bottom_merge_point_first);
		if (!second_merge_point_ready)
			bottom_merge_point_second = safeDecrement(second->size(), bottom_merge_point_second);

	} while (!(first_merge_point_ready && second_merge_point_ready));

	//now we have the correct merge points and can create the new convex Hull
	ConvexHull* newHull = new ConvexHull();

	//iterate from merge point to merge point and fill the new convex hull (in the right order!!!)
	for (int i = bottom_merge_point_first; i != upper_merge_point_first; i = safeIncrement(first->size(), i)) {
		newHull->addPoint(first->at(i));
	}
	newHull->addPoint(first->at(upper_merge_point_first));

	for (int i = upper_merge_point_second; i != bottom_merge_point_second; i = safeIncrement(second->size(), i)) {
		newHull->addPoint(second->at(i));
	}
	newHull->addPoint(second->at(bottom_merge_point_second));


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
