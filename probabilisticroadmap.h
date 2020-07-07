#ifndef PROBABILISTICROADMAP_H
#define PROBABILISTICROADMAP_H

#include "planner.h"

class ProbabilisticRoadMap : public Planner
{
public:
    ProbabilisticRoadMap(Node start_node, Node end_node, OccupancyGrid &occ);
    virtual ~ProbabilisticRoadMap() {}
    virtual vector<vector<int>> find_path();
    pair<int, int> get_random_coordinates();
    vector<Node> find_neighbors(int row, int col, int rad);
};

#endif // PROBABILISTICROADMAP_H
