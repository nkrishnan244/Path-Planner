#include "occupancy_grid.h"
// #include "matplotlibcpp.h"

using namespace std;

OccupancyGrid::OccupancyGrid() {

}

OccupancyGrid::OccupancyGrid(int num_rows, int num_cols, int padding) {
    vector<int> occ_row(num_cols, 0);
    for (int col = 0; col < num_cols; col++) {
        grid.push_back(occ_row);
    }
    size = num_rows;
}

bool OccupancyGrid::add_rectangle_obs(int row, int col, int height, int width) {
    int top_left_row = row - width/2;
    int top_left_col = col - height/2;
    for (int col = top_left_col; col < top_left_col + width; col++) {
        for (int row = top_left_row; row < top_left_row + height; row++) {
            grid.at(row).at(col) = 1;
        }
    }
    rect_obstacles.push_back(vector<int>{row, col, height, width});
    return true;
}
