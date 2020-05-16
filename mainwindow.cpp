#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "occupancy_grid.h"
#include <iostream>
#include "astar.h"
#include "rrt.h"
#include <vector>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int occRows = 60;
    int occCols = 60;
    occSize = {occRows, occCols};
    occ = OccupancyGrid(occRows, occCols, 0);

    ui->setupUi(this);

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(clicked(int)));

    int bSize = 250/sqrt(occRows*occCols);

    const QSize btnSize = QSize(bSize, bSize);
    for (int row = 0; row < occRows*occCols; row++) {
        buttons.push_back(new QPushButton(QString()));
        buttons[row]->setFixedSize(btnSize);
        signalMapper->setMapping(buttons[row], row);
        connect(buttons[row], SIGNAL(clicked()), signalMapper, SLOT(map()));
    }


    for (int row = 0; row < occRows; row++) {
        for (int col = 0; col < occCols; col++) {
            ui->gridLayout->addWidget(buttons[occCols*row + col], row, col);

        }
    }
}

vector<int> get_row_col(int val, int gridSize) {
    int col = val%gridSize;
    int row = (val - col)/gridSize;
    return vector<int>{row, col};
}

void MainWindow::clicked(int text) {
    qDebug() << text;
    QPalette redPal;
    redPal.setColor(QPalette::Button, QColor(Qt::red));

    QPalette whitePal;
    whitePal.setColor(QPalette::Button, QColor(Qt::white));

    QPalette currColor = buttons[text]->palette();

    buttons[text]->setAutoFillBackground(true);

    vector<int> rowCol = get_row_col(text, occSize[0]);

    if (currColor == redPal) {
        occ.grid[rowCol[0]][rowCol[1]] = 0;
        buttons[text]->setPalette(whitePal);
    }
    else {
        occ.grid[rowCol[0]][rowCol[1]] = 1;
        buttons[text]->setPalette(redPal);
    }

    buttons[text]->update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    int x = ui->gridLayoutWidget->geometry().x();
    int y = ui->gridLayoutWidget->geometry().y();

    painter.setPen(QPen(Qt::black, 8, Qt::DashDotLine, Qt::RoundCap));
    if (solvedPts.size() == 0) return;

    for (int i = 1; i < solvedPts[0].size(); i++) {
        int row = solvedPts[0][i];
        int col = solvedPts[1][i];

        QPoint newPoint = buttons[occSize[0]*row + col]->mapToParent(QPoint(0, 0));
        QPoint prevPoint = buttons[occSize[0]*solvedPts[0][i-1] + solvedPts[1][i-1]]->mapToParent(QPoint(0, 0));

        painter.drawLine(x + prevPoint.x(), y + prevPoint.y(), x + newPoint.x(), y + newPoint.y());
    }
}

void MainWindow::makeGrid(vector<vector<int>> &pts) {
    int prevRow = 0;
    int prevCol = 0;

    solvedPts = pts;

    for (int i = 0; i < pts[0].size(); i++) {
        int row = pts[0][i];
        int col = pts[1][i];

        QPalette pal;
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        buttons[occSize[0]*row + col]->setAutoFillBackground(true);
        buttons[occSize[0]*row + col]->setPalette(pal);
        buttons[occSize[0]*row + col]->update();

        prevRow = row;
        prevCol = col;

    }
    update();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset(bool resetObs)
{
    QPalette pal;
    QPalette redPal;
    redPal.setColor(QPalette::Button, QColor(Qt::red));

    pal.setColor(QPalette::Button, QColor(Qt::white));
    for (int i = 0; i < occSize[0]*occSize[1]; i++) {
        buttons[i]->setAutoFillBackground(true);
        if ((resetObs == true) || (buttons[i]->palette() != redPal)) {
            vector<int> rowCol = get_row_col(i, occSize[0]);
            occ.grid[rowCol[0]][rowCol[1]] = 0;
            buttons[i]->setPalette(pal);
        }
        buttons[i]->update();
    }
}


void MainWindow::makePlot(vector<vector<int>> &pts) {
    // generate some data:
    x.clear();
    y.clear();


    int numPts = pts[0].size();
//    QVector<double> x(num_pts), y(num_pts); // initialize with entries 0..100
    for (int i=0; i<numPts; ++i)
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
    ui->customPlot->xAxis->setRange(0, 9);
    ui->customPlot->yAxis->setRange(0, 9);
    ui->customPlot->replot();
    ui->customPlot->update();

}

void MainWindow::on_pushButton_clicked()
{
    reset(false);

    Node start(0, 0, 0, 0);
    Node end(occSize[0] - 1, occSize[0] - 1, 0, 0);

    qDebug() << "Button has been clicked";

    Dijkstra dijkstra(start, end, occ);
    auto startTime = std::chrono::high_resolution_clock::now();
    vector<vector<int>> pts = dijkstra.findPath();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(endTime - startTime);
    string thisString = "Computation time is : ";
    thisString.append(to_string(duration.count()));
    qDebug() << "Duration is : " << duration.count();
    ui->textEdit->setText(QString::fromStdString(thisString));

    makePlot(pts);
    makeGrid(pts);
}

void MainWindow::on_pushButton_2_clicked()
{
    reset(false);
    Node start(0, 0, 0, 0);
    Node end(occSize[0] - 1, occSize[0] - 1, 0, 0);

    Astar astar(start, end, occ);
    auto startTime = std::chrono::system_clock::now();
    vector<vector<int>> pts = astar.findPath();
    auto endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration<double>(endTime - startTime);
    string thisString = "Computation time is : ";
    thisString.append(to_string(duration.count()));
    qDebug() << "Duration is : " << duration.count();
    ui->textEdit->setText(QString::fromStdString(thisString));

    makePlot(pts);
    makeGrid(pts);
}

void MainWindow::on_pushButton_4_clicked()
{
    reset(false);
//    int occ_size = occSize[0];
    Node start(0, 0, 0, 0);
    Node end(occSize[0] - 1, occSize[0] - 1, 0, 0);

    RRT astar(start, end, occ);
    auto startTime = std::chrono::system_clock::now();
    vector<vector<int>> pts = astar.findPath();
    auto endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration<double>(endTime - startTime);
    string thisString = "Computation time is : ";
    thisString.append(to_string(duration.count()));
    qDebug() << "Duration is : " << duration.count();
    ui->textEdit->setText(QString::fromStdString(thisString));

    makePlot(pts);
    makeGrid(pts);
}

void MainWindow::on_pushButton_3_clicked()
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


void MainWindow::on_pushButton_5_clicked()
{
    occ.addRectangleObsTopleft(obsRow, obsCol, obsHeight, obsWidth);
    QPalette pal;
    pal.setColor(QPalette::Button, QColor(Qt::red));
    qDebug() << "MAX ROW IS " << obsRow + obsHeight;
    qDebug() << "MAX COL IS " << obsCol + obsWidth;
    for (int row = obsRow; row <= obsRow + obsHeight; row++) {
        for (int col = obsCol; col <= obsCol + obsWidth; col++) {
            buttons[occSize[0]*row + col]->setPalette(pal);
        }
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    srand(time(NULL));
    int row = rand()%occSize[0];
    int col = rand()%occSize[0];
    int height = rand()%(occSize[0] - row);
    int width = rand()%(occSize[0] - col);
    this->ui->rowText->setPlainText(QString::number(row));
    this->ui->colText->setPlainText(QString::number(col));
    this->ui->heightText->setPlainText(QString::number(height));
    this->ui->widthText->setPlainText(QString::number(width));
}
