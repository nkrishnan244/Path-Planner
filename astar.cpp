#include "astar.h"
#include <math.h>
#include "node.h"

using namespace std;

Astar::Astar(Node start_node, Node end_node, vector<vector<int>> &occ) 
: Dijkstra(start_node, end_node, occ)
{
}

Astar::~Astar()
{
}

float calculate_heuristic(float row, float col, Node end) {
    return sqrt(pow(row - end.get_row(), 2) + pow(col - end.get_col(), 2));
}

void Astar::add_node_to_queue(int row, int col, priority_queue<Node> &pq, Node* parent_node_ptr) {
    if (occ_grid[row][col] == 0) {
        float h = calculate_heuristic(row, col, end); 
        int g = parent_node_ptr->get_g() + 1;
        Node* new_node_ptr = new Node(row, col, g, h); 
        new_node_ptr->val = g + h;
        new_node_ptr->parent = parent_node_ptr;
        pq.push(*new_node_ptr);
    }
}

bool Astar::find_path() {
    return Dijkstra::find_path();
}