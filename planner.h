#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include "node.h"
#include "edge.h"

using namespace std;

class Planner {
    public:

    private:
        Planner(Node start, Node end, vector<vector<int>> occ);
        void find_path(); 
        vector<Node> vertices;
        vector<Edge> edges;
        vector<vector<int>> occ_grid;
};

#endif