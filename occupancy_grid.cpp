#include "occupancy_grid.h"
#include <QDebug>

using namespace std;

OccupancyGrid::OccupancyGrid() {

}

OccupancyGrid::OccupancyGrid(int numRows, int numCols, int padding) {
    vector<int> occRow(numCols, 0);
    for (int col = 0; col < numCols; col++) {
        grid.push_back(occRow);
    }
    size = numRows;
}

bool OccupancyGrid::addRectangleObs(int row, int col, int height, int width) {
    int topLeftRow = row - width/2;
    int topLeftCol = col - height/2;
    for (int col = topLeftCol; col < topLeftCol + width; col++) {
        for (int row = topLeftRow; row < topLeftRow + height; row++) {
            grid.at(row).at(col) = 1;
        }
    }
    return true;
}

bool OccupancyGrid::addRectangleObsTopleft(int inputRow, int inputCol, int height, int width) {
    for (int col = inputCol; col <= inputCol + width; col++) {
        for (int row = inputRow; row <= inputRow + height; row++) {
            grid.at(row).at(col) = 1;
        }
    }
    return true;
}

