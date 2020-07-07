#include "qlearning.h"
#include "occupancy_grid.h"
#include <math.h>
#include <QDebug>
#include <stdlib.h>
#include <queue>

using namespace std;

QLearning::~QLearning()
{
}


float dist(Node start, Node end) {
    return sqrt(pow(start.getRow() - end.getRow(), 2) + pow(start.getCol() - end.getCol(), 2));
}

void QLearning::getActionSpace(vector<vector<float>>& actionSpace, vector<float> qValues, bool includeAll) {
    if (includeAll || qValues[0] >= 0) {
        actionSpace.push_back({-1, 0}); // up
    }
    if (includeAll || qValues[1] >= 0) {
        actionSpace.push_back({0, -1}); // left
    }
    if (includeAll || qValues[2] >= 0) {
        actionSpace.push_back({1, 0}); // down
    }
    if (includeAll || qValues[3] >= 0) {
        actionSpace.push_back({0, 1}); // right
    }
}

int QLearning::rowColToIndex(int row, int col) {
    return (occGrid.size*row + col);
}

bool QLearning::withinBounds(int row, int col) {
    int occSize = occGrid.grid.size();
    if (row < 0 || row >= occSize || col < 0 || col >= occSize) {
        return false;
    }
    return true;
}

void QLearning::trainModel() {

    vector<vector<float>> qTable;


    srand(time( NULL ));

    for (int i = 0; i < 60*60; i++) {
        vector<float> vect({0, 0, 0, 0});
        qTable.push_back(vect); // states are start, end, other
    }

    int numSteps = 0;
    int reward = 0;
    int occSize = occGrid.size;

    int runningNums = 0;
    bool exploring;

    int newState;
    int state;

    float newDist = 0;

    Node currNode(start.getRow(), start.getCol(), 0);

    vector<vector<float>> actionSpace; // get action space
//    getActionSpace(actionSpace);

    int actionIndex;
    vector<float> relevantQVals;

    vector<float> action;

    queue<int> convergenceCheck;

    bool converged = false;

    for (int i = 0; i < maxIterations; i++) {

        if (converged == true) {
            qDebug() << "Alg has converged";
            break;
        }

        qDebug() << "--------------------------------";

        numSteps = 0;

        currNode.setRow(start.getRow()); // reset the state
        currNode.setCol(start.getCol());

        reward = 0;

        exploring = true;

        newState = rowColToIndex(currNode.getRow(), currNode.getCol()); // current state

        action = {};

        bool bestCourse = (rand()%2  > 0); // half the time take the best possible course, other half take a random course

        while (exploring == true) {

            actionSpace = {};

            numSteps += 1;

            if (numSteps > maxSteps) {
                exploring = false;
                continue;
            }

            state = newState;

            relevantQVals = qTable[state];

            float currDist = dist(currNode, end); // get current distance

            if (bestCourse) {

                getActionSpace(actionSpace, qTable[state], true);

                actionIndex = max_element(qTable[state].begin(), qTable[state].end()) - qTable[state].begin();


                action = actionSpace[actionIndex];

            } else {

                getActionSpace(actionSpace, qTable[state], false);

                if (actionSpace.size() == 0) {
                    exploring = false;
                    continue;
                }

                actionIndex = rand()%actionSpace.size();
                action = actionSpace[actionIndex];
            }

            int realIndex;

            if (action == vector<float>{-1, 0}) {
                realIndex = 0;
            }

            else if (action == vector<float>{0, -1}) {
                realIndex = 1;
            }

            else if (action == vector<float>{1, 0}) {
                realIndex = 2;
            }
            else if (action == vector<float>{0, 1}) {
                realIndex = 3;
            }

            int prevRow = currNode.getRow();
            int prevCol = currNode.getCol();

            int newRow = prevRow + action[0];
            int newCol = prevCol + action[1];

            currNode.setRow(newRow);
            currNode.setCol(newCol);

            newDist = dist(currNode, end);

            newState = rowColToIndex(newRow, newCol);

            if (!withinBounds(newRow, newCol)) {
                qTable[state][realIndex] = -1000;
                exploring = false;
            }

            else if (newRow == end.getRow() && newCol == end.getCol()) {
                reward = 1000;
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                float qTableVal = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                qTable[state][realIndex] = qTableVal;
                exploring = false;
            }

            else if (occGrid.grid[newRow][newCol] == 1) {
                reward = -1000;
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                qTable[state][realIndex] = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                exploring = false;
            }

            else if (newDist < currDist) {
                reward = 1;
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                float qTableVal = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                qTable[state][realIndex] = qTableVal;
//                qTable[state][actionIndex] = (1 - learningRate)*qTable[state][actionIndex] + learningRate*(reward + discountFactor*qMax);
            }

            else if (newDist > currDist) {
                reward = 0;
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                qTable[state][realIndex] = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
            }

            else {
                reward = 0;
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                qTable[state][realIndex] = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
            }

        }

        if (bestCourse) {
            convergenceCheck.push(numSteps);

            if (convergenceCheck.size() > 5) {

                runningNums -= convergenceCheck.front();
                runningNums += numSteps;

                convergenceCheck.pop();

                if (runningNums/5 == numSteps && numSteps != maxSteps + 1) {
                    converged = true;
                }
            } else {

                runningNums += numSteps;
            }
        }

        qDebug() << "Number of steps is " << numSteps << "and best course is " << bestCourse;
        qDebug() << "i is " << i;

    }
    qDebug() << "WE HERE";
    table = qTable;
}

vector<Point> QLearning::findPath() {
    int currRow = start.getRow();
    int currCol = start.getCol();

    int currState = 0;

    vector<float> v;
    int actionIndex;

    int numIters = 0;

    vector<Point> pts;

    vector<int> rows;
    vector<int> cols;

    auto startTime = std::chrono::high_resolution_clock::now();

    while ((abs(currRow - end.getRow()) >= 1) || (abs(currCol - end.getCol()) >= 1)) {

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(endTime - startTime);
        if (duration.count() > 30) {
//            qDebug() << "HERE";
        }

        currState = rowColToIndex(currRow, currCol);

        pts.push_back(Point(currRow, currCol));

        rows.push_back(currRow);
        cols.push_back(currCol);

//        qDebug() << "Curr Row is " << currRow;
//        qDebug() << "Curr Col is " << currCol;
//        qDebug() << "Curr State is " << currState;

        v = table[currState];
        actionIndex = max_element(v.begin(),v.end()) - v.begin();
        // 0 = up, 1 = left, 2 = down, right = 3

        if (actionIndex == 0) {
            currRow -= 1;
        }
        else if (actionIndex == 1) {
            currCol -= 1;
        }
        else if (actionIndex == 2) {
            currRow += 1;
        }
        else if (actionIndex == 3) {
            currCol += 1;
        }
    }

    rows.push_back(end.getRow());
    cols.push_back(end.getCol());

    pts.push_back(Point(end.getRow(), end.getCol()));

    reverse(pts.begin(), pts.end());

    reverse(rows.begin(), rows.end());
    reverse(cols.begin(), cols.end());

    vector<vector<int>> thisPath = {rows, cols};

    return pts;

}

QLearning::QLearning(Node startNode, Node endNode, OccupancyGrid occ)
{
    start = startNode;
    end = endNode;
    occGrid = occ;
}

//                qTable[state][actionIndex] = (1 - learningRate)*qTable[state][actionIndex] + learningRate*(reward + discountFactor*)
