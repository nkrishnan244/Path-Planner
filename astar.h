#ifndef ASTAR_H
#define ASTAR_H

#include "dijkstra.h"
#include <math.h>
#include "plotter.h"
// #include "node.h"

using namespace std;

class Astar : public Dijkstra {
    public:
        Astar(Node start_node, Node end_node, OccupancyGrid &occ);
        ~Astar();
        vector<vector<int>> find_path();

    // private:
        void add_node_to_queue(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr);

};

#endif
