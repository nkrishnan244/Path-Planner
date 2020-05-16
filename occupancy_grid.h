#ifndef OCCUPANCY_GRID_H
#define OCCUPANCY_GRID_H

// #include "plotter.h"
#include <vector>

using namespace std;

class OccupancyGrid {
    public:
        OccupancyGrid();
        OccupancyGrid(int numRows, int numCols, int padding);
        bool addRectangleObs(int row, int col, int height, int width);
        bool addRectangleObsTopleft(int inputRow, int inputCol, int height, int width);

        vector<vector<int>> grid;
        int numCols;
        int size;
};

#endif
