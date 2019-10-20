#include <iostream>
#include "dijkstra.h"
// #include "node.h"
// #include "node.cpp" // Need to get rid of this
#include <math.h>
#include "matplotlibcpp.h"
#include <unordered_set>

using namespace std;
namespace plt = matplotlibcpp;

Dijkstra::Dijkstra(Node start_node, Node end_node, vector<vector<int>> &occ) 
// : start(start_node), end(end_node), occ_grid(occ)
: Planner(start_node, end_node, occ)
{
       
}

void Dijkstra::add_node_to_queue(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr) {
    if (occ_grid[row][col] == 0) {
        // float h = calculate_heuristic(row, col, end); 
        int g = parent_node_ptr->get_g() + 1;
        Node* new_node_ptr = new Node(row, col, g);
        new_node_ptr->val = g;  
        new_node_ptr->parent = parent_node_ptr;
        pq.push(*new_node_ptr);
    }

}

namespace std 
{
template<>
    struct hash<Node>
    {
        size_t
        operator()(const Node &node) const
        {
            return hash<int>()(node.get_row()) ^ hash<int>()(node.get_col()); 
        }
    };
}

// plt::plot({1,3,2,4});
// plt::show();

// Must implement
bool Dijkstra::find_path() {
    unordered_set<Node> seen_nodes;
    priority_queue<Node> pqueue;
    // Node curr_node = start;
    Node* curr_node_ptr = &start;

    while (*curr_node_ptr != end) {
        seen_nodes.insert(*curr_node_ptr); 
        int curr_row = curr_node_ptr->get_row();
        int curr_col = curr_node_ptr->get_col();

        // Check down
        if (curr_row < occ_grid.size() - 1) {
            add_node_to_queue(curr_row + 1, curr_col, pqueue, curr_node_ptr);
        }

        // Check up 
        if (curr_row > 0) {
            add_node_to_queue(curr_row - 1, curr_col, pqueue, curr_node_ptr);
        }

        // Check right
        if (curr_col < occ_grid.size() - 1) {
            add_node_to_queue(curr_row, curr_col + 1, pqueue, curr_node_ptr);
        }

        // Check left
        if (curr_col > 0) {
            add_node_to_queue(curr_row, curr_col - 1, pqueue, curr_node_ptr);
        }

        Node next_node = pqueue.top();
        pqueue.pop();

        while (seen_nodes.find(next_node) != seen_nodes.end()) {

            next_node = pqueue.top();
            pqueue.pop();
        }
        
        curr_node_ptr = new Node(next_node.get_row(), next_node.get_col(), next_node.get_g(), next_node.get_h());
        curr_node_ptr->parent = next_node.parent;

        // cout << "--------------------------------------------------------------------------------------\n";
        // cout << "Curr node Row is : " << curr_node_ptr->get_row() << " Curr node Col is : " << curr_node_ptr->get_col() << "\n"; 
        // cout << "Parent node Row is : " << curr_node_ptr->get_parent()->get_row() << " Parent node Col is : " << curr_node_ptr->get_parent()->get_col() << "\n"; 
    }
    // while (curr_node.get_x() != 1 && curr_node.get_y() != 1) {

    cout << "--------------------------------------------------------------------------------------\n";
    // Node curr_node = *curr_node_ptr;

    vector<int> final_rows;
    vector<int> final_cols;

    while (*curr_node_ptr != start) {
            // Node* parent = curr_node_ptr->get_parent();
            cout << "Curr node row is : " << curr_node_ptr->get_row() << " Curr node col is : " << curr_node_ptr->get_col() << "\n"; 
            curr_node_ptr = curr_node_ptr->get_parent();
            final_rows.push_back(curr_node_ptr->get_row());
            final_cols.push_back(curr_node_ptr->get_col());
    }
    plt::plot(final_rows, final_cols);
    plt::show();

    cout << "Curr node row is : " << curr_node_ptr->get_row() << " Curr node col is : " << curr_node_ptr->get_col() << "\n"; 
    return true; 
}