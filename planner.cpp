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
    // Node curr_node = start;
    Node* curr_node_ptr = &start;
    // cout << "Finding Path";
    while (1) {
        cout << "Curr node X is : " << curr_node_ptr->get_x() << " Curr node Y is : " << curr_node_ptr->get_y() << "\n"; 
    // while (*curr_node_ptr != end) {
        // cout << "Curr node X is : " << curr_node_ptr->get_x() << " Curr node Y is : " << curr_node_ptr->get_y() << "\n"; 

        int curr_row = curr_node_ptr->get_x();
        int curr_col = curr_node_ptr->get_y();

        // cout << "Curr node X is : " << curr_node_ptr->get_x() << " Curr node Y is : " << curr_node_ptr->get_y() << "\n"; 

        // cout << "CURR ROW IS " << curr_row << "\n";
        // cout << "CURR COL IS " << curr_col << "\n";
        // int curr_row = 0;
        // int curr_col = 0;

        // cout << "Parent node X is : " << curr_node.get_parent()->get_x() << " Parent node Y is : " << curr_node.get_parent()->get_y() << "\n"; 

        // Check down
        // cout << "Checking Down";
        // cout << "Curr node X is : " << curr_node.get_x() << " Curr node Y is : " << curr_node.get_y() << "\n"; 

        if (curr_row > 0) {
            if (occ_grid[curr_row - 1][curr_col] == 0) {
                float h = calculate_heuristic(curr_col, curr_row - 1, end); 
                int g = curr_node_ptr->get_g() + 1;
                Node down_node(curr_col, curr_row - 1, g, h);
                down_node.set_parent(*curr_node_ptr);
                pqueue.push(down_node);
            }
        }

        // Check up
        if (curr_row < occ_grid.size()) {
            if (occ_grid[curr_row + 1][curr_col] == 0) {
                float h = calculate_heuristic(curr_col, curr_row + 1, end); 
                int g = curr_node_ptr->get_g() + 1;
                Node up_node(curr_col, curr_row + 1, g, h);
                up_node.set_parent(*curr_node_ptr);
                // cout << "Curr node X is : " << curr_node.get_x() << " Curr node Y is : " << curr_node.get_y() << "\n"; 
                // cout << "Up node X is : " << up_node.get_x() << " Up node Y is : " << up_node.get_y() << "\n"; 
                // cout << "Parent node X is : " << up_node.get_parent()->get_x() << " Parent node Y is : " << up_node.get_parent()->get_y() << "\n"; 
                pqueue.push(up_node);
                // Node next_node = pqueue.top();
                // cout << "Curr node X is : " << next_node.get_x() << " Curr node Y is : " << next_node.get_y() << "\n"; 
                // cout << "Parent node X is : " << next_node.get_parent()->get_x() << " Parent node Y is : " << next_node.get_parent()->get_y() << "\n"; 
                // pqueue.pop();
            }
        }

        // Check left
        if (curr_col > 0) {
            if (occ_grid[curr_row][curr_col - 1] == 0) {
                float h = calculate_heuristic(curr_col - 1, curr_row, end); 
                int g = curr_node_ptr->get_g() + 1;
                Node left_node(curr_col - 1, curr_row, g, h);
                left_node.set_parent(*curr_node_ptr);
                // cout << "Curr node X is : " << curr_node.get_x() << " Curr node Y is : " << curr_node.get_y() << "\n"; 
                // cout << "Left node X is : " << left_node.get_x() << " Left node Y is : " << left_node.get_y() << "\n"; 
                // cout << "Parent node X is : " << left_node.get_parent()->get_x() << " Parent node Y is : " << left_node.get_parent()->get_y() << "\n"; 
                pqueue.push(left_node);
            }
        }

        // Check right
        if (curr_col < occ_grid.size()) {
            if (occ_grid[curr_row][curr_col + 1] == 0) {
                float h = calculate_heuristic(curr_col + 1, curr_row, end); 
                int g = curr_node_ptr->get_g() + 1;
                Node right_node(curr_col + 1, curr_row, g, h);
                right_node.set_parent(*curr_node_ptr);
                // cout << "Curr node X is : " << curr_node.get_x() << " Curr node Y is : " << curr_node.get_y() << "\n"; 
                // cout << "Right node X is : " << right_node.get_x() << " Right node Y is : " << right_node.get_y() << "\n"; 
                // cout << "Parent node X is : " << right_node.get_parent()->get_x() << " Parent node Y is : " << right_node.get_parent()->get_y() << "\n"; 
                pqueue.push(right_node);
            }
        }
        Node next_node = pqueue.top();
        Node* curr_node_ptr = &next_node;

        pqueue.pop();

        cout << "--------------------------------------------------------------------------------------\n";
        cout << "Curr node X is : " << curr_node_ptr->get_x() << " Curr node Y is : " << curr_node_ptr->get_y() << "\n"; 
        // cout << "Parent node X is : " << curr_node_ptr->get_parent()->get_x() << " Parent node Y is : " << curr_node_ptr->get_parent()->get_y() << "\n"; 
        int i = 0;
        while (i < 2000000000) {
                i++;
        }
        cout << "Curr node X is : " << curr_node_ptr->get_x() << " Curr node Y is : " << curr_node_ptr->get_y() << "\n"; 
    }
    // while (curr_node.get_x() != 1 && curr_node.get_y() != 1) {


    // while (curr_node != start) {
    //         Node* parent = curr_node.get_parent();
    //         cout << "Parent node X is : " << parent->get_x() << " Parent node Y is : " << parent->get_y() << "\n"; 
    //         int i = 0;
    //         while (i < 100000000000) {
    //             i++;
    //         }
    //         curr_node = *parent;
    // }
    return true; 
}