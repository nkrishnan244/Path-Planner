#include <iostream>
#include "planner.h"

using namespace std;

Planner::Planner(Node start, Node end, vector<vector<int>> occ) 
: occ_grid(occ)
{
    vertices.push_back(start); 
    vertices.push_back(end);     
}

// Must implement
void Planner::find_path() {
    return; 
}