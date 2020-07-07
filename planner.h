#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include "node.h"
#include "occupancy_grid.h"
// #include "matplotlibcpp.h"

using namespace std;

class Planner {
    public:
        Planner();
        Planner(Node startNode, Node endNode, OccupancyGrid &occ);
        virtual vector<Point> findPath()=0;

    // private:
        Node start;
        Node end;
        OccupancyGrid occGrid;
};

#endif
