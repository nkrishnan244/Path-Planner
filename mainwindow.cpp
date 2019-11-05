#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "occupancy_grid.h"
#include <iostream>
#include "astar.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void make_plot(vector<vector<int>> &pts, OccupancyGrid &obs) {

}


void MainWindow::make_plot(vector<vector<int>> &pts) {
    // generate some data:
    x.clear();
    y.clear();

    int num_pts = pts[0].size();
//    QVector<double> x(num_pts), y(num_pts); // initialize with entries 0..100
    for (int i=0; i<num_pts; ++i)
    {
        x.push_back(pts[0][i]);
        y.push_back(pts[1][i]);
    }
//    qv_x.append(1);

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 128);
    ui->customPlot->yAxis->setRange(0, 128);
    ui->customPlot->replot();
    ui->customPlot->update();

}

void MainWindow::on_pushButton_clicked()
{
    int occ_size = 128;
    Node start(0, 0, 0, 0);
    Node end(occ_size - 1, occ_size - 1, 0, 0);

    cout << "Button has been clicked";
    OccupancyGrid occ(occ_size, occ_size, 0);
    occ.add_rectangle_obs(64, 64, 30, 30);

    Dijkstra astar(start, end, occ);
    vector<vector<int>> pts = astar.find_path();

    make_plot(pts);

//    MainWindow plotter;
//    plotter.make_plot(pts);
//    plotter.show();
}

void MainWindow::on_pushButton_2_clicked()
{
    int occ_size = 128;
    Node start(0, 0, 0, 0);
    Node end(occ_size - 1, occ_size - 1, 0, 0);

    cout << "Button has been clicked";
    OccupancyGrid occ(occ_size, occ_size, 0);
    occ.add_rectangle_obs(64, 64, 30, 30);

    Astar astar(start, end, occ);
    vector<vector<int>> pts = astar.find_path();

    make_plot(pts);
}
