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

Planner::Planner(Node start_node, Node end_node, OccupancyGrid &occ) 
: start(start_node), end(end_node), occ_grid(occ)
{

}