#include <iostream>
#include "planner.h"
#include <math.h>

using namespace std;

Planner::Planner()
{
}

Planner::Planner(Node startNode, Node endNode, OccupancyGrid &occ)
: start(startNode), end(endNode), occGrid(occ)
{
}
