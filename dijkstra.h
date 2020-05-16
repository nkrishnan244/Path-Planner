#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "occupancy_grid.h"
#include <queue>
#include "planner.h"
#include <unordered_set>
#include "plotter.h"

class Dijkstra : public Planner {
    public:
        virtual ~Dijkstra() {}
        Dijkstra(Node startNode, Node endNode, OccupancyGrid &occ);
        virtual vector<vector<int>> findPath();
        virtual void addNodeToQueue(unsigned int row, unsigned int col, priority_queue<Node> &pq, Node* parentNodePtr);
};

#endif
