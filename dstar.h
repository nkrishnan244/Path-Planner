#ifndef DSTAR_H
#define DSTAR_H

#include <vector>
#include "occupancy_grid.h"
#include <queue>
#include "planner.h"
#include <unordered_set>
#include "plotter.h"
#include "dijkstra.h"

class Dstar : public Dijkstra
{
public:
    virtual ~Dstar() {}
    Dstar(Node startNode, Node endNode, OccupancyGrid &occ);
    vector<Point> findPath();
    virtual void setNodeValue(Node &newNode);
    void addNodeToQueue(int row, int col, priority_queue<Node> &pq, Node* parentNodePtr);


//    const double maxSolveTime = 10.0;
//    const double diagonalDistance = 1.414;
//    const double nonDiagonalDistance = 1.0;
};

#endif // DSTAR_H
