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
        vector<Point> findPath();
        virtual void setNodeValue(Node &newNode);
        void addNodeToQueue(int row, int col, priority_queue<Node> &pq, Node* parentNodePtr);

        const double maxSolveTime = 10.0;
        const double diagonalDistance = 1.414;
        const double nonDiagonalDistance = 1.0;
};

#endif
