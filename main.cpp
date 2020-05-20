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
    int occSize = 3;
    Node start(0, 0, 0, 0);
    Node end(occSize - 1, occSize - 1, 0, 0);

    // vector<vector<int>> occ;

    OccupancyGrid occ(occSize, occSize, 0);
//    occ.add_rectangle_obs(64, 64, 30, 30);

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
