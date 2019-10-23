#ifndef OCCUPANCY_GRID_H
#define OCCUPANCY_GRID_H

// #include "plotter.h"
#include <vector>

using namespace std; 

class OccupancyGrid {
    public:
        OccupancyGrid();
        OccupancyGrid(int num_rows, int num_cols, int padding); 
        bool add_rectangle_obs(int row, int col, int height, int width);

        vector<vector<int>> grid; 
};

#endif
