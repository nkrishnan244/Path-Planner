#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "occupancy_grid.h"
#include <iostream>
#include "astar.h"
#include "rrt.h"
#include <vector>
#include <sstream>
#include "planner.h"
#include "dijkstra.h"
#include "astar.h"
#include <fstream>
#include <iterator>
#include "qlearning.h"
#include "point.h"

// Right now, the size of the occupancy grid is fixed and can not be edited by the user
static int OCC_ROWS = 60;
static int OCC_COLS = OCC_ROWS; // for now we always assume the grid is square so these can both be used
static vector<int> OCC_SIZE{OCC_ROWS, OCC_COLS};

static int BUTTON_SIZE = 4;

static float MOTION_SLIDER_SLOPE = -48.0;
static float MOTION_SLIDER_INTERCEPT = 5000.0;

// Maps button vector index values to a row, col pair
Point indexValToRowCol(int index) {
    int col = index%OCC_ROWS;
    int row = (index - col)/OCC_ROWS;
    return Point(row, col);
}

// Maps a row, col pair to a button vector index value
int rowColToIndexVal(int row, int col) {
    return OCC_COLS*row + col;
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // initialize colors
    redPalette.setColor(QPalette::Button, QColor(Qt::red));
    whitePalette.setColor(QPalette::Button, QColor(Qt::white));
    bluePalette.setColor(QPalette::Button, QColor(Qt::blue));

    stage = "planning"; // variable used to signal if we are planning or actually moving

    occ = OccupancyGrid(OCC_ROWS, OCC_COLS, 0);

    ui->setupUi(this);

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(clicked(int))); // !! add comment explaining this !!

    const QSize btnSize = QSize(BUTTON_SIZE, BUTTON_SIZE);

    // Create a vector of buttons (pointers) to represent each button in the 2d grid
    for (int buttonNum = 0; buttonNum < OCC_ROWS*OCC_COLS; buttonNum++) {
        buttons.push_back(new QPushButton(QString())); // buttons aren't labeled -> maybe add numbers in the future
        buttons[buttonNum]->setFixedSize(btnSize);
        signalMapper->setMapping(buttons[buttonNum], buttonNum);
        connect(buttons[buttonNum], SIGNAL(clicked()), signalMapper, SLOT(map())); // !! add comment explaining this !!
    }

    // Place the buttons on the grid layout
    for (int row = 0; row < OCC_ROWS; row++) {
        for (int col = 0; col < OCC_COLS; col++) {
            ui->gridLayout->addWidget(buttons[rowColToIndexVal(row, col)], row, col); // adds a button at the specified row and column position
        }
    }

    ui->motionSpeedSlider->setSliderPosition(60); // initial slider position for motion speed

    // create timer for updating motion
    motionTimer = new QTimer(this);
    connect(motionTimer, SIGNAL(timeout()), this, SLOT(incrementPoint()));

    srand(time(NULL)); // randomly seed the random number generator
}

// when a button is clicked, toggle color and add to/remove from occ grid
void MainWindow::clicked(int buttonIndex) {
    qDebug() << buttonIndex;

    QPalette buttonColor = buttons[buttonIndex]->palette();

    buttons[buttonIndex]->setAutoFillBackground(true);

    Point buttonPos = indexValToRowCol(buttonIndex);

    if (buttons[buttonIndex]->palette() == redPalette) {
        occ.grid.at(buttonPos.getRow()).at(buttonPos.getCol()) = 0;
        buttons[buttonIndex]->setPalette(whitePalette);
    }
    else {
        occ.grid.at(buttonPos.getRow()).at(buttonPos.getCol()) = 1;
        buttons[buttonIndex]->setPalette(redPalette);
    }

    buttons[buttonIndex]->update();
}


// Paints on top of the graphs
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // If no points have been plotted
    if (solvedPts.size() == 0) return;

    // get the top left position of the grid Layout
    int topLeftX = ui->gridLayoutWidget->geometry().x();
    int topLeftY = ui->gridLayoutWidget->geometry().y();

    for (int i = 1; i < solvedPts.size(); i++) {

        // For all points up to our current point, paint blue. For the rest, paint black
        // If we have not started moving, all points will be black
        if (i > solvedPts.size() - lastPoint - 1) {
            painter.setPen(QPen(Qt::blue, 8, Qt::DashDotLine, Qt::RoundCap));
        } else {
            painter.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));
        }

        int row = solvedPts.at(i).getRow();
        int col = solvedPts.at(i).getCol();

        // draw line between new point and previous point
        QPoint newPoint = buttons[rowColToIndexVal(row, col)]->mapToParent(QPoint(0, 0));
        QPoint prevPoint = buttons[rowColToIndexVal(solvedPts.at(i-1).getRow(), solvedPts.at(i-1).getCol())]->mapToParent(QPoint(0, 0));

        painter.drawLine(topLeftX + prevPoint.x(), topLeftY + prevPoint.y(), topLeftX + newPoint.x(), topLeftY + newPoint.y());
    }

}

// Plot the solved pts on the grid (in case we don't want to draw a line over the points)
void MainWindow::makeGrid(vector<Point> &pts) {
    // set global solved pts variable as pts
    solvedPts = pts;

    for (int i = 0; i < pts.size(); i++) {
        int row = pts.at(i).getRow();
        int col = pts.at(i).getCol();

        buttons[rowColToIndexVal(row, col)]->setAutoFillBackground(true);
        buttons[rowColToIndexVal(row, col)]->setPalette(bluePalette);
        buttons[rowColToIndexVal(row, col)]->update();
    }
    update();
}

// Reset the grid (and obstacles if resetObs==true)
void MainWindow::reset(bool resetObs)
{
    for (int i = 0; i < OCC_ROWS*OCC_COLS; i++) {
        buttons[i]->setAutoFillBackground(true);
        if ((resetObs == true) || (buttons[i]->palette() != redPalette)) {
            Point buttonPos = indexValToRowCol(i);
            occ.grid.at(buttonPos.getRow()).at(buttonPos.getCol()) = 0;
            buttons[i]->setPalette(whitePalette);
        }
        buttons[i]->update();
    }
}

bool MainWindow::startPlanning(string plannerAlg, Point startPoint, vector<Point> existingPath) {
    reset(false); // clear previous points
    Node start(startPoint.getRow(), startPoint.getCol(), 0, 0);
    Node end(OCC_ROWS - 1, OCC_COLS - 1, 0, 0); // right now end point is bottom left, would be nice to change it
    plannerName = plannerAlg; // set global variable

    auto startTime = std::chrono::high_resolution_clock::now();

    vector<Point> pts;

    if (plannerAlg == "Dijkstra") {
        qDebug() << "Planning Dijkstra...";
        Dijkstra planner(start, end, occ);
        pts = planner.findPath();
    } else if (plannerAlg == "Astar") {
        qDebug() << "Planning A*...";
        Astar planner(start, end, occ);
        pts = planner.findPath();
    } else if (plannerAlg == "RRT") {
        qDebug() << "Planning RRT...";
        RRT planner(start, end, occ);
        pts = planner.findPath();
    } else if (plannerAlg == "Q-Learning") {
        qDebug() << "Planning Q-Learning...";
        QLearning planner(start, end, occ);
        planner.trainModel();
        pts = planner.findPath();
    }

    if (pts.size() == 0) {
        ui->textEdit->setText(QString::fromStdString("Error: No path could be found!"));
        return false;
    }

    qDebug() << "Finished Planning!";

    // Combine existing path already traveled on and new path
    if (existingPath.size() != 0) {

        vector<Point> finalPts;

        finalPts.reserve(pts.size() + existingPath.size());
        finalPts.insert(finalPts.end(), pts.begin(), pts.end());

        for (int i = 0; i < existingPath.size(); i++) {
            finalPts.push_back(Point(existingPath[existingPath.size() - 1 - i].getRow(), existingPath[existingPath.size() - 1 - i].getCol()));
        }

        pts = finalPts;
    }

    // display computation time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(endTime - startTime);
    string thisString = "Computation time is : ";
    thisString.append(to_string(duration.count()));
    ui->textEdit->setText(QString::fromStdString(thisString));

    makeGrid(pts);

    return true;
}

void MainWindow::on_dijkstraButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("Dijkstra", Point(0, 0), {});
}

void MainWindow::on_aStarButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("Astar", Point(0, 0), {});
}

void MainWindow::on_rrtButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("RRT", Point(0, 0), {});
}

void MainWindow::on_qLearningButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("Q-Learning", Point(0, 0), {});
}

void MainWindow::on_resetButton_clicked()
{
    reset(true);
}

void MainWindow::on_rowText_textChanged()
{
    obsRow = this->ui->rowText->toPlainText().toInt();
    qDebug() << "setting obstacle row to " << obsRow;
}

void MainWindow::on_colText_textChanged()
{
    obsCol = this->ui->colText->toPlainText().toInt();
    qDebug() << "setting obstacle col to " << obsCol;
}

void MainWindow::on_heightText_textChanged()
{
    obsHeight = this->ui->heightText->toPlainText().toInt();
    qDebug() << "setting obstacle height to " << obsHeight;
}

void MainWindow::on_widthText_textChanged()
{
    obsWidth = this->ui->widthText->toPlainText().toInt();
    qDebug() << "setting obstacle width to " << obsWidth;
}

// Insert obstacle
void MainWindow::on_insertObstacleButton_clicked()
{
    if (obsCol + obsWidth >= OCC_COLS) {
        ui->textEdit->setText(QString::fromStdString("Error: Obstacle is too wide!"));
        return;
    }
    if (obsRow + obsHeight >= OCC_ROWS) {
        ui->textEdit->setText(QString::fromStdString("Error: Obstacle is too tall!"));
        return;
    }

    for (int row = obsRow; row < obsRow + obsHeight; row++) {
        for (int col = obsCol; col < obsCol + obsWidth; col++) {
            buttons[rowColToIndexVal(row, col)]->setPalette(redPalette);
            occ.grid.at(row).at(col) = 1;
        }
    }
    ui->textEdit->setText(QString::fromStdString("Success: Obstacle has been added!"));
}

// randomly provide obstacle dimensions for the user to input
void MainWindow::on_randomizeButton_clicked()
{
    int row = rand()%OCC_ROWS;
    int col = rand()%OCC_COLS;
    int height = rand()%(OCC_ROWS - row);
    int width = rand()%(OCC_COLS - col);
    this->ui->rowText->setPlainText(QString::number(row));
    this->ui->colText->setPlainText(QString::number(col));
    this->ui->heightText->setPlainText(QString::number(height));
    this->ui->widthText->setPlainText(QString::number(width));
}

bool MainWindow::checkObstacle(vector<Point> &pts) {
    int numPoints = pts.size();
    // points are in reverse order...it might be better to change that at some point
    for (int i=lastPoint; i<numPoints; i++) {
        int row = pts.at(numPoints - 1 - i).getRow();
        int col = pts.at(numPoints - 1 - i).getCol();
        if (occ.grid.at(row).at(col) == 1) {
            return true;
        }
    }
    return false;
}

// Increments our point over time during motion
void MainWindow::incrementPoint() {
    bool obs = checkObstacle(solvedPts);

    // If there is an obstacle, replan!
    if (obs) {
        Point initPoint(solvedPts.at(solvedPts.size() - lastPoint - 1).getRow(), solvedPts.at(solvedPts.size() - lastPoint - 1).getCol());

        // If the current point is covered, we can't replan
        if (occ.grid.at(initPoint.getRow()).at(initPoint.getCol()) == 1) {
            qDebug() << "current point is covered, can't solve!";
            motionTimer->stop();
            return;
        }

        qDebug("PATH IS OBSTRUCTED....replanning");

        vector<Point> initialPts;

        for (int i = 0; i < lastPoint; i++) {
            initialPts.push_back(Point(solvedPts.at(solvedPts.size() - i - 1).getRow(), solvedPts.at(solvedPts.size() - i - 1).getCol()));
        }

        bool planSuccessful = startPlanning(plannerName, initPoint, initialPts);

        if (!planSuccessful) {
            motionTimer->stop();
            qDebug() << "couldn't solve the issue";
            return;
        }
    }

    lastPoint+=1;
    qDebug() << "last point is " << lastPoint;

    if (lastPoint == solvedPts.size() - 1) {
        motionTimer->stop();
        lastPoint = 0;
        qDebug() << "Done Moving!";
    }
    update();
}

float getMotionSliderSpeed(int currPos) {
    return MOTION_SLIDER_SLOPE*currPos + MOTION_SLIDER_INTERCEPT;
}

// Start Moving Button
void MainWindow::on_startMovingButton_clicked()
{
    if (solvedPts.size() == 0) {
        ui->textEdit->setText(QString::fromStdString("Error: No path has been planned!"));
        return;
    }

    if (stage != "Moving") {
        lastPoint = 0;
    }

    stage = "Moving";

    int position = this->ui->motionSpeedSlider->value();
    qDebug() << "UPDATING EVERY " << getMotionSliderSpeed(position)/1000.0;
    motionTimer->start(getMotionSliderSpeed(position));
//    update();
}

// Pause movement
void MainWindow::on_pausePlayButton_clicked()
{
    if (solvedPts.size() == 0) {
        ui->textEdit->setText(QString::fromStdString("Error: No path has been planned!"));
        return;
    }

    if (motionTimer->isActive()) {
        motionTimer->stop();
    } else {
        int currPos = this->ui->motionSpeedSlider->value();
        qDebug() << "UPDATING EVERY " << (getMotionSliderSpeed(currPos))/1000.0;
        motionTimer->start(getMotionSliderSpeed(currPos));
    }
}

void MainWindow::on_planningSpeedSlider_sliderMoved(int position)
{
    qDebug() << "Slider Position is " << position;
}


void MainWindow::on_motionSpeedSlider_sliderMoved(int position)
{
    motionTimer->stop();
    qDebug() << "UPDATING EVERY " << (getMotionSliderSpeed(position))/1000.0;
    motionTimer->start(getMotionSliderSpeed(position));
}

void MainWindow::on_printPathButton_clicked()
{
    if (solvedPts.size() == 0) {
        return;
    }

    for (int i = 0; i < solvedPts.size(); i++) {
        qDebug() << "Row is " << solvedPts.at(i).getRow() << ", Col is " << solvedPts.at(i).getCol();
    }
}
