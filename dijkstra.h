#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "occupancy_grid.h"
#include <queue>
#include "planner.h"
#include <unordered_set>
#include "plotter.h"

// using namespace std;

class Dijkstra : public Planner {
    public:
        virtual ~Dijkstra() {}
        Dijkstra(Node start_node, Node end_node, OccupancyGrid &occ);
        virtual vector<vector<int>> find_path();

    // private:
        // Node start;
        // Node end;
        // vector<vector<int>> occ_grid;
    // private:
        virtual void add_node_to_queue(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr);
};

#endif
