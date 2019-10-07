#include <iostream>
#include "planner.h"
#include <queue> 
#include "node.h"
// #include "node.cpp" // Need to get rid of this
#include <math.h>

using namespace std;

Planner::Planner(Node start, Node end, vector<vector<int>>& occ) 
: start(start), end(end), occ_grid(occ)
{
       
}

float calculate_heuristic(float x, float y, Node end) {
    return sqrt(pow(x - end.get_x(), 2) + pow(y - end.get_y(), 2));
}

bool operator<(const Node& lhs, const Node& rhs) { 
  return lhs.get_h() > rhs.get_h();
}

// Must implement
bool Planner::find_path() {
    priority_queue<Node> pqueue;
    Node curr_node = start;
    while (curr_node != end) {
        int curr_row = curr_node.get_x();
        int curr_col = curr_node.get_y();

        // Check down
        if (curr_row > 0) {
            if (occ_grid[curr_row - 1][curr_col] == 0) {
                float h = calculate_heuristic(curr_col, curr_row - 1, end); 
                int g = curr_node.get_g() + 1;
                pqueue.push(Node(curr_col, curr_row - 1, g, h));
            }
        }

        // Check up
        if (curr_row < occ_grid.size()) {
            if (occ_grid[curr_row + 1][curr_col] == 0) {
                float h = calculate_heuristic(curr_col, curr_row + 1, end); 
                int g = curr_node.get_g() + 1;
                pqueue.push(Node(curr_col, curr_row + 1, g, h));
            }
        }

        // Check left
        if (curr_col > 0) {
            if (occ_grid[curr_row][curr_col - 1] == 0) {
                float h = calculate_heuristic(curr_col - 1, curr_row, end); 
                int g = curr_node.get_g() + 1;
                pqueue.push(Node(curr_col - 1, curr_row, g, h));
            }
        }

        // Check right
        if (curr_col < occ_grid.size()) {
            if (occ_grid[curr_row][curr_col + 1] == 0) {
                float h = calculate_heuristic(curr_col + 1, curr_row, end); 
                int g = curr_node.get_g() + 1;
                pqueue.push(Node(curr_col + 1, curr_row, g, h));
            }
        }
        cout << "Curr node X is : " << curr_node.get_x() << " Curr node Y is : " << curr_node.get_y() << "\n"; 
        curr_node = pqueue.top();
        pqueue.pop();
    }
    return true; 
}