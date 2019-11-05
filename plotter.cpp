#include <iostream>
#include "matplotlibcpp.h"
#include "plotter.h"


namespace plt = matplotlibcpp;

using namespace std; 

Plotter::Plotter() 
{

}

bool Plotter::plot_path(vector<int> &rows, vector<int> &cols) {
    plt::plot(rows, cols);
    plt::show();
    return true;
}

bool Plotter::plot_occupancy_grid(OccupancyGrid &occ_grid) {
    // Prepare data
    int ncols = occ_grid.grid.size();
    int nrows = occ_grid.grid.size();
    std::vector<float> z(ncols * nrows);
    for (int j=0; j<nrows; ++j) {
        for (int i=0; i<ncols; ++i) {
            z.at(ncols * j + i) = occ_grid.grid[j][i]; 
        }
    }

    const float* zptr = &(z[0]);
    const int colors = 1;
    plt::title("A*");
    plt::imshow(zptr, nrows, ncols, colors);
    plt::show();

    return true; 
}

bool Plotter::plot_path_in_grid(OccupancyGrid &occ_grid, vector<int> &rows, vector<int> &cols) {
    cout << "num points are " << rows.size() << "\n";
    for (int index = 0; index < rows.size(); index++) {
        occ_grid.grid[rows[index]][cols[index]] = 2; 
    }
    plot_occupancy_grid(occ_grid);
    return true; 
}