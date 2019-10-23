#include <iostream>
#include "dijkstra.h"
#include "astar.h"
#include "node.h"
#include "occupancy_grid.h"
#include "plotter.h"

// #include "node.cpp" 
// #include "planner.cpp"

using namespace std;

int main() {
    int occ_size = 128;
    Node start(0, 0, 0, 0);
    Node end(occ_size - 1, occ_size - 1, 0, 0);

    // vector<vector<int>> occ; 

    OccupancyGrid occ(occ_size, occ_size, 0);
    occ.add_rectangle_obs(64, 64, 30, 30);   


    // Plotter plotter;
    // plotter.plot_occupancy_grid(occ);

    Astar astar(start, end, occ);
    astar.find_path();

    // Dijkstra dijkstra(start, end, occ);
    // dijkstra.find_path();

    cout << "Finished planning!";
    return 0;
}