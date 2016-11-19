#pragma once
#include <vector>

#include "Point.h"
#include "ConvexHull.h"
#include "I_View.hpp"

/**
Divide And Conquer Algorithm preparation
*@param points - the vector with all the vertices we want to iterate over -> they will be sorted (x scale)
*/
void DaC_preparation(std::vector<Point*>* points);

/**
Divide And Conquer Algorithm recursive step
*@param points - the vector with all the vertices we want to iterate over
@param ch - te convex hull object, that will be filled/updated
*/
ConvexHull* DaC_step(std::vector<Point*>* points, int begin, int end, I_View* view = nullptr);