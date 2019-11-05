#ifndef RRT_H
#define RRT_H

#include "planner.h"
#include "node.h"
#include "occupancy_grid.h"
#include <vector>


class RRT : public Planner
{
    public:
        RRT(Node start_node, Node end_node, OccupancyGrid &occ);
        virtual ~RRT() {}
        virtual vector<vector<int>> find_path();
        pair<int, int> get_random_coordinates();
        Node* find_closest_node(int rand_row, int rand_col);

        vector<Node> nodes;
};

#endif // RRT_H
