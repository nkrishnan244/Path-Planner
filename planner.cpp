#include <iostream>
#include "planner.h"
// #include "node.h"
// #include "node.cpp" // Need to get rid of this
#include <math.h>
// #include "matplotlibcpp.h"

using namespace std;
// namespace plt = matplotlibcpp;

Planner::Planner()
{

}

Planner::Planner(Node startNode, Node endNode, OccupancyGrid &occ)
: start(startNode), end(endNode), occGrid(occ)
{

}
