#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include "node.h"

using namespace std;

class Planner {
    public:
        Planner(); 
        Planner(Node start_node, Node end_node, vector<vector<int>> &occ);
        virtual bool find_path() = 0;

    // private: 
        Node start;
        Node end;
        vector<vector<int>> occ_grid;
};

#endif