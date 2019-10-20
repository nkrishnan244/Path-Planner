#include <iostream>
#include "planner.h"
#include "node.h"
// #include "node.cpp" // Need to get rid of this
#include <math.h>

using namespace std;

Planner::Planner(Node start, Node end, vector<vector<int>>& occ) 
: start(start), end(end), occ_grid(occ)
{
       
}

float calculate_heuristic(float row, float col, Node end) {
    return sqrt(pow(row - end.get_row(), 2) + pow(col - end.get_col(), 2));
}

void Planner::add_node(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr) {
    if (occ_grid[row][col] == 0) {
        float h = calculate_heuristic(row, col, end); 
        int g = parent_node_ptr->get_g() + 1;
        Node* new_node_ptr = new Node(row, col, g, h); 
        new_node_ptr->parent = parent_node_ptr;
        pq.push(*new_node_ptr);
    }

}

bool operator<(const Node& lhs, const Node& rhs) { 
  return lhs.get_h() > rhs.get_h();
}


// Must implement
bool Planner::find_path() {
    priority_queue<Node> pqueue;
    // Node curr_node = start;
    Node* curr_node_ptr = &start;

    while (*curr_node_ptr != end) {
        int curr_row = curr_node_ptr->get_row();
        int curr_col = curr_node_ptr->get_col();

        // Check down
        if (curr_row < occ_grid.size() - 1) {
            add_node(curr_row + 1, curr_col, pqueue, curr_node_ptr);
        }

        // Check up 
        if (curr_row > 0) {
            add_node(curr_row - 1, curr_col, pqueue, curr_node_ptr);
        }

        // Check right
        if (curr_col < occ_grid.size() - 1) {
            add_node(curr_row, curr_col + 1, pqueue, curr_node_ptr);
        }

        // Check left
        if (curr_col > 0) {
            add_node(curr_row, curr_col - 1, pqueue, curr_node_ptr);
        }

        Node next_node = pqueue.top();
        curr_node_ptr = new Node(next_node.get_row(), next_node.get_col(), next_node.get_g(), next_node.get_h());
        curr_node_ptr->parent = next_node.parent;

        pqueue.pop();

        cout << "--------------------------------------------------------------------------------------\n";
        cout << "Curr node Row is : " << curr_node_ptr->get_row() << " Curr node Col is : " << curr_node_ptr->get_col() << "\n"; 
        cout << "Parent node Row is : " << curr_node_ptr->get_parent()->get_row() << " Parent node Col is : " << curr_node_ptr->get_parent()->get_col() << "\n"; 
    }
    // while (curr_node.get_x() != 1 && curr_node.get_y() != 1) {

    cout << "--------------------------------------------------------------------------------------\n";
    // Node curr_node = *curr_node_ptr;
    while (*curr_node_ptr != start) {
            // Node* parent = curr_node_ptr->get_parent();
            cout << "Curr node row is : " << curr_node_ptr->get_row() << " Curr node col is : " << curr_node_ptr->get_col() << "\n"; 
            curr_node_ptr = curr_node_ptr->get_parent();
    }
    cout << "Curr node row is : " << curr_node_ptr->get_row() << " Curr node col is : " << curr_node_ptr->get_col() << "\n"; 
    return true; 
}