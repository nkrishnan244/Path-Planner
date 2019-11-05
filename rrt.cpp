#include "rrt.h"
#include "planner.h"
#include <stdlib.h>
#include <limits>
#include <math.h>

RRT::RRT(Node start_node, Node end_node, OccupancyGrid &occ)
: Planner(start_node, end_node, occ)
{

}

pair<int, int> RRT::get_random_coordinates() {
    int occ_size = occ_grid.size;
    return pair<int, int>(rand()%occ_size, rand()%occ_size);
}

float dist(Node node, int row, int col) {
    return sqrt(pow(node.get_row() - row, 2) + pow(node.get_col() - col, 2));
}

Node* RRT::find_closest_node(int rand_row, int rand_col) {
    float min_dist = numeric_limits<float>::max();
    Node* closest_node = new Node();
    for (Node& node:nodes) {
        float curr_dist = dist(node, rand_row, rand_col);
        if (curr_dist < min_dist) {
            closest_node = &node;
            min_dist = curr_dist;
        }
    }
    return closest_node;
}

vector<vector<int>> RRT::find_path() {
    Node* curr_node_ptr = &start;
    while (*curr_node_ptr != end) {
        pair<int, int> curr_coord = get_random_coordinates();
        int rand_row = curr_coord.first;
        int rand_col = curr_coord.second;
        if (occ_grid.grid[rand_row][rand_col] == 0) {
            Node* closest_node = find_closest_node(rand_row, rand_col);
        }
    }
}
