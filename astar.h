#ifndef ASTAR_H
#define ASTAR_H

#include "dijkstra.h"
#include <math.h>
#include "plotter.h"
// #include "node.h"

using namespace std;

class Astar : public Dijkstra {
    public:
        Astar(Node startNode, Node endNode, OccupancyGrid &occ);
        ~Astar();

        void setNodeValue(Node &newNode);
        double calculateHeuristic(double row, double col);
        void addNodeToQueue(int row, int col, priority_queue<Node> &pq, Node* parentNodePtr);
};

#endif
