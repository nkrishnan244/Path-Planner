#include <iostream>
#include "dijkstra.h"
#include "astar.h"
#include "node.h"

// #include "node.cpp" 
// #include "planner.cpp"

using namespace std;

int main() {
    int occ_size = 32;
    Node start(0, 0, 0, 0);
    Node end(occ_size - 1, occ_size - 1, 0, 0);

    vector<vector<int>> occ(occ_size); 

    vector<int> curr_row(occ_size, 0);
    for (int col = 0; col < occ_size; col++) {
        occ.at(col) = curr_row;
    }
    // cout << "HERE";

    Astar astar(start, end, occ);
    astar.find_path();

    Dijkstra dijkstra(start, end, occ);
    dijkstra.find_path();

    cout << "Finished planning!";
    return 0;
}