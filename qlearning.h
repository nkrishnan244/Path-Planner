#ifndef QLEARNING_H
#define QLEARNING_H

#include <occupancy_grid.h>
#include <node.h>

class QLearning
{
public:
    QLearning(Node start, Node end, OccupancyGrid occ);
    OccupancyGrid occGrid;
    ~QLearning();

    void trainModel();

    void getActionSpace(vector<vector<float>>& actionSpace, vector<float> qValues, bool includeAll);

    int rowColToIndex(int row, int col);
    bool withinBounds(int row, int col);

    vector<vector<int>> findPath();

    Node start;
    Node end;

    vector<vector<float>> table;
};

#endif // QLEARNING_H
