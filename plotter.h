#ifndef PLOTTER_H
#define PLOTTER_H

#include "occupancy_grid.h" 
#include <vector>

using namespace std;

class Plotter {
    public:
        Plotter();
        bool plot_path(vector<int> &rows, vector<int> &cols);
        bool plot_occupancy_grid(OccupancyGrid &occ_grid);
        bool plot_path_in_grid(OccupancyGrid &occ_grid, vector<int> &rows, vector<int> &cols);
};

#endif