#ifndef QLEARNING_H
#define QLEARNING_H

#include <occupancy_grid.h>
#include <node.h>
#include <point.h>
#include <planner.h>

class QLearning : public Planner
{
public:
    QLearning(Node start, Node end, OccupancyGrid occ);
    ~QLearning();

    void trainModel();

    void getActionSpace(vector<vector<float>>& actionSpace, vector<float> qValues, bool includeAll);

    int rowColToIndex(int row, int col);
    bool withinBounds(int row, int col);

    vector<Point> findPath();

    vector<vector<float>> table;

    const double maxIterations = 100000;
    const double maxSteps = 20000;
    const double learningRate = 0.01;
    const double discountFactor = 0.9;
};

#endif // QLEARNING_H
