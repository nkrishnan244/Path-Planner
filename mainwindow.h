#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "occupancy_grid.h"
#include <QPushButton>
#include <QSignalMapper>
#include <iostream>
#include "planner.h"
#include "point.h"

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
    void makeGrid(vector<Point> &pts);

    bool startPlanning(string planner, Point startPoint, vector<Point> existingPath);
    bool checkObstacle(vector<Point> &pts);

    void reset(bool);

    QPalette redPalette;
    QPalette whitePalette;
    QPalette bluePalette;


    OccupancyGrid occ;
    vector<Point> solvedPts;
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
    void on_dijkstraButton_clicked();
    void on_aStarButton_clicked();
    void on_resetButton_clicked();
    void on_rrtButton_clicked();
    void clicked(int text);

    void on_rowText_textChanged();
    void on_colText_textChanged();
    void on_heightText_textChanged();
    void on_widthText_textChanged();

    void on_insertObstacleButton_clicked();

    void on_randomizeButton_clicked();



    void on_startMovingButton_clicked();

    void incrementPoint();

    void on_pausePlayButton_clicked();

    void on_planningSpeedSlider_sliderMoved(int position);

    void on_motionSpeedSlider_sliderMoved(int position);

    void on_printPathButton_clicked();

    void on_qLearningButton_clicked();

    void on_dStarButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> x, y;
    QVector<QPushButton*> buttons;
    QSignalMapper *signalMapper;
};
#endif // MAINWINDOW_H
