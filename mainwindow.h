#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "occupancy_grid.h"
#include <QPushButton>
#include <QSignalMapper>
#include <iostream>
#include "planner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void makePlot(vector<vector<int>> &pts);
    void makePlot(vector<vector<int>> &pts, OccupancyGrid &obs);
    void makeGrid(vector<vector<int>> &pts);

    bool startPlanning(string planner, vector<int> startPoint, vector<vector<int>> existingPath);
    bool checkObstacle(vector<vector<int>> &pts);

    void reset(bool);


    OccupancyGrid occ;
    vector<int> occSize;
    vector<vector<int>> solvedPts;
    int obsRow;
    int obsCol;
    int obsHeight;
    int obsWidth;

    // used for plotting live (hopefully I will find a better way to deal with paintEvent)
    chrono::milliseconds initTime;
    string stage;
    string plannerName;
    int lastPoint;
    QTimer* motionTimer;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void clicked(int text);

    void on_rowText_textChanged();
    void on_colText_textChanged();
    void on_heightText_textChanged();
    void on_widthText_textChanged();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();



    void on_pushButton_8_clicked();

    void incrementPoint();

    void on_pushButton_9_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QVector<double> x, y;
    QVector<QPushButton*> buttons;
    QSignalMapper *signalMapper;
};
#endif // MAINWINDOW_H