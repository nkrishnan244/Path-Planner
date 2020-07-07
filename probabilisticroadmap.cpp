#include "probabilisticroadmap.h"
#include <math.h>
#include <stdlib.h>
#include <limits>
#include "node.h"

ProbabilisticRoadMap::ProbabilisticRoadMap(Node start_node, Node end_node, OccupancyGrid &occ)
: Planner(start_node, end_node, occ)
{

}

pair<int, int> ProbabilisticRoadMap::get_random_coordinates() {
    int occ_size = occGrid.size;
    return pair<int, int>(rand()%occ_size, rand()%occ_size);
}

vector<Node> ProbabilisticRoadMap::find_neighbors(int row, int col, int rad) {
    vector<Node> neighbors;
    if (row > rad - 1)
        neighbors.push_back(Node(row - rad, col));
    if (row < occ_grid.size - rad)
        neighbors.push_back(Node(row + rad, col));
    if (col > rad - 1)
        neighbors.push_back(Node(row, col - rad));
    if (col < occ_grid.size - rad)
        neighbors.push_back(Node(row, col + rad));
    return neighbors;
}

vector<vector<int>> ProbabilisticRoadMap::find_path() {
    vector<Node> vertices;
    int radius = 1;
    int max_iters = 100;
    for (int iter = 0; iter < max_iters; iter++) {
        pair<int, int> curr_coord = get_random_coordinates();
        int rand_row = curr_coord.first;
        int rand_col = curr_coord.second;
        vector<Node> neighbors = find_neighbors(rand_row, rand_col, radius);
    }
}
