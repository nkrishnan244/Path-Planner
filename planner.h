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
        Planner(Node start_node, Node end_node, OccupancyGrid &occ);
        virtual vector<vector<int>> find_path() = 0;

    // private:
        Node start;
        Node end;
        OccupancyGrid occ_grid;
};

#endif
