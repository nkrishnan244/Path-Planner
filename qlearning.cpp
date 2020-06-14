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
    return sqrt(pow(start.get_row() - end.get_row(), 2) + pow(start.get_col() - end.get_col(), 2));
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

    double learningRate = 0.01;
    double discountFactor = 0.9;

    for (int i = 0; i < 60*60; i++) {
        vector<float> vect({0, 0, 0, 0});
        qTable.push_back(vect); // states are start, end, other
    }

    int numIterations = 10000;
    int numSteps = 0;
    int reward = 0;
    int occSize = occGrid.size;

    int runningNums = 0;

    int maxSteps = 2000000;

    bool exploring;

    int newState;
    int state;

    float newDist = 0;

    Node currNode(start.get_row(), start.get_col(), 0);

    vector<vector<float>> actionSpace; // get action space
//    getActionSpace(actionSpace);

    int actionIndex;
    vector<float> relevantQVals;

    vector<float> action;

    queue<int> convergenceCheck;

    bool converged = false;

    for (int i = 0; i < numIterations; i++) {

        if (converged == true) {
            qDebug() << "Alg has converged";
            break;
        }

        qDebug() << "--------------------------------";

        numSteps = 0;

        currNode.set_row(start.get_row()); // reset the state
        currNode.set_col(start.get_col());

        reward = 0;

        exploring = true;

//        qDebug() << "EARLYYYY NEW STATE IS " << newState;

        newState = rowColToIndex(currNode.get_row(), currNode.get_col()); // current state

//        qDebug() << "EARLY NEW STATE IS " << newState;

        action = {};

//        qDebug() << "RE INITIALIZE ACTION";

        bool bestCourse = (rand()%2  > 0); // half the time take the best possible course, other half take a random course

        if (bestCourse) {
            maxSteps = 20000;
        } else {
            maxSteps = 20000;
        }

//        qDebug() << "BEST COURSE IS " << bestCourse;

//        qDebug() << "RE INITIALIZE bestCourse";
        while (exploring == true) {

            actionSpace = {};

            numSteps += 1;

            if (numSteps > maxSteps) {
                exploring = false;
                continue;
            }

//            qDebug() << "PRE STATE = NEWSTATE";

            state = newState;

//            qDebug() << "POST STATE = NEWSTATE";
//            qDebug() << "State is " << state;

            relevantQVals = qTable[state];

            float currDist = dist(currNode, end); // get current distance

            if (bestCourse) {

//                qDebug() << "BESTCOURSE";

                getActionSpace(actionSpace, qTable[state], true);

                actionIndex = max_element(qTable[state].begin(), qTable[state].end()) - qTable[state].begin();
//                if (actionIndex == 4) {
//                    qDebug();
//                }
//                qDebug() << "action index is " << actionIndex;
//                qDebug() << "len of actionSpace is " << actionSpace.size();

                action = actionSpace[actionIndex];

            } else {

//                qDebug() << "!BESTCOURSE";

                getActionSpace(actionSpace, qTable[state], false);

                if (actionSpace.size() == 0) {
                    exploring = false;
                    continue;
                }

                actionIndex = rand()%actionSpace.size();
                action = actionSpace[actionIndex];
            }

//            qDebug() << "POST BESTCOURSE LOGIC";

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

//            qDebug() << "Real Index is " << realIndex;

//            vector<float> action = actionSpace[actionIndex]; // randomly pick an action

            int prevRow = currNode.get_row();
            int prevCol = currNode.get_col();

            int newRow = prevRow + action[0];
            int newCol = prevCol + action[1];

            currNode.set_row(newRow);
            currNode.set_col(newCol);

            newDist = dist(currNode, end);

            newState = rowColToIndex(newRow, newCol);

//            qDebug() << "WE ARE WITHIN " << (withinBounds(newRow, newCol));
//            qDebug() << "Prev row is " << prevRow;
//            qDebug() << "Prev col is " << prevCol;

            if (!withinBounds(newRow, newCol)) {
                qTable[state][realIndex] = -1000;
                exploring = false;
            }

            else if (newRow == end.get_row() && newCol == end.get_col()) {
                reward = 1000;
//                qDebug() << "A";
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                float qTableVal = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                qTable[state][realIndex] = qTableVal;
                exploring = false;
            }

            else if (occGrid.grid[newRow][newCol] == 1) {
                reward = -1000;
//                qDebug() << "B";
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                qTable[state][realIndex] = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                exploring = false;
            }

            else if (newDist < currDist) {
                reward = 1;
//                qDebug() << "C";
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                float qTableVal = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
                qTable[state][realIndex] = qTableVal;
//                qTable[state][actionIndex] = (1 - learningRate)*qTable[state][actionIndex] + learningRate*(reward + discountFactor*qMax);
            }

            else if (newDist > currDist) {
                reward = 0;
//                qDebug() << "D";
                float qMax = *max_element(qTable[newState].begin(), qTable[newState].end());
                qTable[state][realIndex] = (1 - learningRate)*qTable[state][realIndex] + learningRate*(reward + discountFactor*qMax);
            }

            else {
                reward = 0;
//                qDebug() << "E";
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

vector<vector<int>> QLearning::findPath() {
    int currRow = start.get_row();
    int currCol = start.get_col();

    int currState = 0;

    vector<float> v;
    int actionIndex;

    int numIters = 0;

    vector<int> rows;
    vector<int> cols;

    auto startTime = std::chrono::high_resolution_clock::now();

    while ((abs(currRow - end.get_row()) >= 1) || (abs(currCol - end.get_col()) >= 1)) {

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(endTime - startTime);
        if (duration.count() > 30) {
//            qDebug() << "HERE";
        }

        currState = rowColToIndex(currRow, currCol);

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

    rows.push_back(end.get_row());
    cols.push_back(end.get_col());

    reverse(rows.begin(), rows.end());
    reverse(cols.begin(), cols.end());

    vector<vector<int>> thisPath = {rows, cols};

    return thisPath;

}

QLearning::QLearning(Node startNode, Node endNode, OccupancyGrid occ)
{
    start = startNode;
    end = endNode;
    occGrid = occ;
}

//                qTable[state][actionIndex] = (1 - learningRate)*qTable[state][actionIndex] + learningRate*(reward + discountFactor*)
