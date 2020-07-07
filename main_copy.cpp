#include <iostream>
#include "dijkstra.h"
#include "astar.h"
#include "node.h"
#include "occupancy_grid.h"
#include "mainwindow.h"
#include <QApplication>
//#include "plotter.h"

// #include "node.cpp"
// #include "planner.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    int occ_size = 128;
    Node start(0, 0, 0, 0);
    Node end(occ_size - 1, occ_size - 1, 0, 0);

    // vector<vector<int>> occ;

    OccupancyGrid occ(occ_size, occ_size, 0);
    occ.add_rectangle_obs(64, 64, 30, 30);

    QApplication a(argc, argv);
//    MainWindow plotter;
//    plotter.make_plot();
//    plotter.show();

    // Plotter plotter;
    // plotter.plot_occupancy_grid(occ);

//    Astar astar(start, end, occ);
//    vector<vector<int>> pts = astar.find_path();

    MainWindow plotter;
//    plotter.make_plot(pts);
    plotter.show();

    // Dijkstra dijkstra(start, end, occ);
    // dijkstra.find_path();

//    MainWindow plotter;
//    plotter.make_plot(pts);
//    plotter.show();

    cout << "Finished planning!";
    return a.exec();
}
