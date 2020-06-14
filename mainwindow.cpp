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

// Right now, the size of the occupancy grid is fixed and can not be edited by the user
static int OCC_ROWS = 60;
static int OCC_COLS = OCC_ROWS; // for now we always assume the grid is square so these can both be used
static vector<int> OCC_SIZE{OCC_ROWS, OCC_COLS};

static int BUTTON_SIZE = 4;

// Add colors to the project
static QPalette RED_PALETTE;
static QPalette WHITE_PALETTE;
static QPalette BLUE_PALETTE;

// Maps button vector index values to a row, col pair
pair<int, int> indexValToRowCol(int index) {
    int col = index%OCC_ROWS;
    int row = (index - col)/OCC_ROWS;
    return pair<int, int>{row, col};
}

// Maps a row, col pair to a button vector index value
int rowColToIndexVal(int row, int col) {
    return OCC_COLS*row + col;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // initialize colors
    RED_PALETTE.setColor(QPalette::Button, QColor(Qt::red));
    WHITE_PALETTE.setColor(QPalette::Button, QColor(Qt::white));
    BLUE_PALETTE.setColor(QPalette::Button, QColor(Qt::blue));

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
}

// when a button is clicked, toggle color and add to/remove from occ grid
void MainWindow::clicked(int buttonIndex) {
    qDebug() << buttonIndex;

    QPalette buttonColor = buttons[buttonIndex]->palette();

    buttons[buttonIndex]->setAutoFillBackground(true);

    pair<int, int> rowCol = indexValToRowCol(buttonIndex);

    if (buttonColor == RED_PALETTE) {
        occ.grid.at(rowCol.first).at(rowCol.second) = 0;
        buttons[buttonIndex]->setPalette(WHITE_PALETTE);
    }
    else {
        occ.grid.at(rowCol.first).at(rowCol.second) = 1;
        buttons[buttonIndex]->setPalette(RED_PALETTE);
    }

    buttons[buttonIndex]->update();
}


// Paints on top of thegraphs
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (stage != "Moving") {

        // get the top left position of the grid Layout
        int topLeftX = ui->gridLayoutWidget->geometry().x();
        int topLeftY = ui->gridLayoutWidget->geometry().y();

        painter.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));
        if (solvedPts.size() == 0) return;

        // draw all the points after mapping them to the global frame
        for (int i = 1; i < solvedPts.at(0).size(); i++) {
            int row = solvedPts.at(0).at(i);
            int col = solvedPts.at(1).at(i);

            QPoint newPoint = buttons.at(rowColToIndexVal(row, col))->mapToParent(QPoint(0, 0));
            QPoint prevPoint = buttons.at(rowColToIndexVal(solvedPts[0][i-1], solvedPts[1][i-1]))->mapToParent(QPoint(0, 0));

            painter.drawLine(topLeftX + prevPoint.x(), topLeftY + prevPoint.y(), topLeftX + newPoint.x(), topLeftY + newPoint.y());
        }
    }

    else {

        int topLeftX = ui->gridLayoutWidget->geometry().x();
        int topLeftY = ui->gridLayoutWidget->geometry().y();

        if (solvedPts.size() == 0) return;

        for (int i = 1; i < solvedPts[0].size(); i++) {

            // For all points up to our current point, paint blue. For the rest, paint black
            if (i > solvedPts[0].size() - lastPoint - 1) {
                painter.setPen(QPen(Qt::blue, 8, Qt::DashDotLine, Qt::RoundCap));
            } else {
                painter.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));
            }

            int row = solvedPts[0][i];
            int col = solvedPts[1][i];

            QPoint newPoint = buttons[rowColToIndexVal(row, col)]->mapToParent(QPoint(0, 0));
            QPoint prevPoint = buttons[rowColToIndexVal(solvedPts[0][i-1], solvedPts[1][i-1])]->mapToParent(QPoint(0, 0));

            painter.drawLine(topLeftX + prevPoint.x(), topLeftY + prevPoint.y(), topLeftX + newPoint.x(), topLeftY + newPoint.y());
        }
    }
}

// Plot the solved pts on the grid (in case we don't want to draw a line over the points)
void MainWindow::makeGrid(vector<vector<int>> &pts) {
    int prevRow = 0;
    int prevCol = 0;

    solvedPts = pts;

    for (int i = 0; i < pts[0].size(); i++) {
        int row = pts[0][i];
        int col = pts[1][i];

        buttons[rowColToIndexVal(row, col)]->setAutoFillBackground(true);
        buttons[rowColToIndexVal(row, col)]->setPalette(BLUE_PALETTE);
        buttons[rowColToIndexVal(row, col)]->update();

        prevRow = row;
        prevCol = col;
    }
    update();
}

// Destructor (should probably move this)
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset(bool resetObs)
{
    for (int i = 0; i < OCC_ROWS*OCC_COLS; i++) {
        buttons[i]->setAutoFillBackground(true);
        if ((resetObs == true) || (buttons[i]->palette() != RED_PALETTE)) {
            pair<int, int> rowCol = indexValToRowCol(i);
            occ.grid.at(rowCol.first).at(rowCol.second) = 0;
            buttons[i]->setPalette(WHITE_PALETTE);
        }
        buttons[i]->update();
    }
}

bool MainWindow::startPlanning(string plannerAlg, vector<int> startPoint, vector<vector<int>> existingPath) {
    reset(false);
    Node start(startPoint[0], startPoint[1], 0, 0);
    Node end(OCC_ROWS - 1, OCC_COLS - 1, 0, 0);
    plannerName = plannerAlg;

    auto startTime = std::chrono::high_resolution_clock::now();

    vector<vector<int>> pts;

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

//    for (int ptIndex = 0; ptIndex < pts.size(); ptIndex++) {
//        qDebug() << "Row is " << pts[ptIndex][0] << ", Col is " << pts[ptIndex][1];
//    }

    qDebug() << "Finished Planning!";

    if (existingPath.size() != 0) {

        vector<int> finalRows;
        vector<int> finalCols;

        finalRows.reserve(pts[0].size() + existingPath[0].size());
        finalCols.reserve(pts[0].size() + existingPath[0].size());

        finalRows.insert(finalRows.end(), pts[0].begin(), pts[0].end());
        finalCols.insert(finalCols.end(), pts[1].begin(), pts[1].end());

        for (int i = 0; i < existingPath[0].size(); i++) {
            finalRows.push_back(existingPath[0][existingPath[0].size() - 1 - i]);
            finalCols.push_back(existingPath[1][existingPath[0].size() - 1 - i]);
        }

        pts = {finalRows, finalCols};
    }

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
    startPlanning("Dijkstra", {0, 0}, {});
}

void MainWindow::on_aStarButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("Astar", {0, 0}, {});
}

void MainWindow::on_rrtButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("RRT", {0, 0}, {});
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
    occ.addRectangleObsTopleft(obsRow, obsCol, obsHeight, obsWidth);
    QPalette pal;
    pal.setColor(QPalette::Button, QColor(Qt::red));
    for (int row = obsRow; row <= obsRow + obsHeight; row++) {
        for (int col = obsCol; col <= obsCol + obsWidth; col++) {
            buttons[rowColToIndexVal(row, col)]->setPalette(pal);
        }
    }
    ui->textEdit->setText(QString::fromStdString("Success: Obstacle has been added!"));

}

void MainWindow::on_randomizeButton_clicked()
{
    srand(time(NULL));
    int row = rand()%OCC_ROWS;
    int col = rand()%OCC_COLS;
    int height = rand()%(OCC_ROWS - row);
    int width = rand()%(OCC_COLS - col);
    this->ui->rowText->setPlainText(QString::number(row));
    this->ui->colText->setPlainText(QString::number(col));
    this->ui->heightText->setPlainText(QString::number(height));
    this->ui->widthText->setPlainText(QString::number(width));
}

bool MainWindow::checkObstacle(vector<vector<int>> &pts) {
    int numPoints = pts[0].size();
    for (int i=lastPoint; i<numPoints; i++) {
        int row = pts[0][numPoints - 1 - i];
        int col = pts[1][numPoints - 1 - i];
        if (occ.grid.at(row).at(col) == 1) {
            return true;
        }
    }
    return false;
}

void MainWindow::incrementPoint() {
    bool obs = checkObstacle(solvedPts);
    if (obs) {
        vector<int> initPoint = {solvedPts[0][solvedPts[0].size() - lastPoint - 1], solvedPts[1][solvedPts[0].size() - lastPoint - 1]};
        if (occ.grid.at(initPoint[0]).at(initPoint[1]) == 1) {
            qDebug() << "current point is covered, can't solve!";
            motionTimer->stop();
            return;
        }
        vector<int> rows;
        vector<int> cols;
        for (int i = 0; i < lastPoint; i++) {
            rows.push_back(solvedPts[0][solvedPts[0].size() - i - 1]);
            cols.push_back(solvedPts[1][solvedPts[0].size() - i - 1]);
        }
        vector<vector<int>> initialPts = {rows, cols};
        bool planSuccessful = startPlanning(plannerName, initPoint, initialPts);
        if (!planSuccessful) {
            motionTimer->stop();
            qDebug() << "couldn't solve the issue";
            return;
        }
        qDebug("PATH IS OBSTRUCTED....aborting");
    }
    lastPoint+=1;
    if (lastPoint == solvedPts[0].size() - 1) {
        motionTimer->stop();
        lastPoint = 0;
        qDebug() << "Done Moving!";
    }
    qDebug() << "last point is " << lastPoint;
    update();
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

    int currPos = this->ui->motionSpeedSlider->value();
    qDebug() << "UPDATING EVERY " << (-48.0*currPos + 5000.0)/1000.0;
    motionTimer->start(-48*currPos + 5000);
//    update();
}

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
        qDebug() << "UPDATING EVERY " << (-48.0*currPos + 5000.0)/1000.0;
        motionTimer->start(-48*currPos + 5000);
    }
}

void MainWindow::on_planningSpeedSlider_sliderMoved(int position)
{
    qDebug() << "Slider Position is " << position;
}


void MainWindow::on_motionSpeedSlider_sliderMoved(int position)
{
    motionTimer->stop();
    qDebug() << "UPDATING EVERY " << (-48.0*position + 5000.0)/1000.0;
    motionTimer->start(-48*position + 5000);
}

void MainWindow::on_printPathButton_clicked()
{
    if (solvedPts.size() == 0) {
        return;
    }

    for (int i = 0; i < solvedPts[0].size(); i++) {
        qDebug() << "Row is " << solvedPts[0][i] << ", Col is " << solvedPts[1][i];
    }
}

void MainWindow::on_qLearningButton_clicked()
{
    stage = "Planning";
    motionTimer->stop();
    startPlanning("Q-Learning", {0, 0}, {});

    qDebug() << " WE HERE TOO";
}
