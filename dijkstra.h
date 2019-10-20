#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "node.h"
#include "edge.h"
#include <queue> 
#include "planner.h"
#include <unordered_set>

using namespace std;

class Dijkstra : public Planner {
    public:
        Dijkstra(Node start_node, Node end_node, vector<vector<int>> &occ);
        virtual bool find_path(); 

    // private: 
        // Node start;
        // Node end;
        // vector<vector<int>> occ_grid;
    // private:
        virtual void add_node_to_queue(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr);
};

#endif