#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include "node.h"
#include "edge.h"
#include <queue> 

using namespace std;

class Planner {
    public:
        Planner(Node start, Node end, vector<vector<int>>& occ);
        bool find_path();

    private: 
        Node start;
        Node end;
        vector<vector<int>> occ_grid;
        void add_node(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr);
};

#endif