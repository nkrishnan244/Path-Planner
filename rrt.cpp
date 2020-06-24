#include "rrt.h"
#include "planner.h"
#include <stdlib.h>
#include <limits>
#include <math.h>
#include <iostream>
#include <QDebug>

RRT::RRT(Node startNode, Node endNode, OccupancyGrid &occ)
: Planner(startNode, endNode, occ)
{
}

pair<int, int> RRT::getRandomCoordinates() {
    int occSize = occGrid.size;
    return pair<int, int>(rand()%occSize, rand()%occSize);
}

double dist(Node node, int row, int col) {
    return sqrt(pow(node.getRow() - row, 2) + pow(node.getCol() - col, 2));
}

double dist(Node* node, int row, int col) {
    return sqrt(pow(node->getRow() - row, 2) + pow(node->getCol() - col, 2));
}

Node* RRT::findClosestNode(int randRow, int randCol) {
    double minDist = numeric_limits<double>::max();
    Node* closestNode;
    for (Node* node:nodes) {
        double curr_dist = dist(node, randRow, randCol);
        if (curr_dist < minDist) {
            closestNode = node;
            minDist = curr_dist;
        }
    }
    return closestNode;
}

pair<int, int> getIncrements(Node* startNode, Node* endNode) {
    int rowIncrement = 0;
    int colIncrement = 0;

    if (startNode->getRow() > endNode->getRow()) {
        rowIncrement = -1;
    }
    else if (startNode->getRow() < endNode->getRow()) {
        rowIncrement = 1;
    }

    if (startNode->getCol() > endNode->getCol()) {
        colIncrement = -1;
    }

    else if (startNode->getCol() < endNode->getCol()) {
        colIncrement = 1;
    }

    return pair<int, int>(rowIncrement, colIncrement);
}

void RRT::addIntermediateNodes(vector<Point> &nodes, Node* childNode, Node* parentNode) {

    int rowIncrement, colIncrement;
    tie(rowIncrement, colIncrement) = getIncrements(childNode, parentNode);

    int col = childNode->getCol();
    int row = childNode->getRow();

    while (col != parentNode->getCol()+colIncrement || row != parentNode->getRow()+rowIncrement) {
        nodes.push_back(Point(row, col)); // look into making this funciton call modular
        col += colIncrement;
        row += rowIncrement;
    }
}

bool RRT::checkObstacle(Node* startNode, Node* endNode) {
    int rowIncrement = 0;
    int colIncrement = 0;
    tie(rowIncrement, colIncrement) = getIncrements(startNode, endNode);

    int row = startNode->getRow();
    int col = startNode->getCol();

    while (row != endNode->getRow() + rowIncrement || col != endNode->getCol() + colIncrement) {
        if (occGrid.grid[row][col] == 1) {
            return true;
        }
        row += rowIncrement;
        col += colIncrement;
    }
    return false;
}

vector<Point> RRT::findPath() {
    Node* currNode = &start;
    Node* closestNode;
    nodes.push_back(currNode);

    int rrtDist = 7;
    int margin = 0;

    srand(time(NULL));

    while (dist(end, currNode->getRow(), currNode->getCol()) > rrtDist + margin) {

        pair<int, int> currCoord = getRandomCoordinates();
        int randRow = currCoord.first;
        int randCol = currCoord.second;

        if (occGrid.grid[randRow][randCol] == 0) {
            Node* closestNode = findClosestNode(randRow, randCol);

            Node* newNode = new Node(0, 0);
            int rowDirection = randRow - closestNode->getRow();
            int colDirection = randCol - closestNode->getCol();

            int rrtDistRow = 0;
            int rrtDistCol = 0;

            if (abs(rowDirection) > abs(colDirection)) {
                if (rowDirection < 0) {
                    rrtDistRow = -1;
                } else {
                    rrtDistRow = 1;
                }
            }

            else if (abs(rowDirection) < abs(colDirection)) {
                if (colDirection < 0) {
                    rrtDistCol = -1;
                } else {
                    rrtDistCol = 1;
                }
            }

            else {
                if (rowDirection < 0) {
                    rrtDistRow = -1;
                } else {
                    rrtDistRow = 1;
                }

                if (colDirection < 0) {
                    rrtDistCol = -1;
                } else {
                    rrtDistCol = 1;
                }
            }

            newNode->setRow(closestNode->getRow() + rrtDistRow*rrtDist);
            newNode->setCol(closestNode->getCol() + rrtDistCol*rrtDist);

            // Make sure that this a new point
            if (rowDirection == 0 && colDirection == 0) continue;

            // Make sure we are within the bounds of the occ grid
            if (newNode->getRow() >= occGrid.grid.size() || newNode->getCol() >= occGrid.grid.size())
                continue;
            if (newNode->getRow() < 0 || newNode->getCol() < 0)
                continue;

            bool isObstacle = checkObstacle(closestNode, newNode);

            if (!isObstacle) {
                newNode->parent = closestNode;
                currNode = newNode;
                nodes.push_back(newNode);

            }
        }
    }

    qDebug() << "FINISHED PLANNING";

    vector<Point> pts;

//    pts.push_back(Point(end.getRow(), end.getCol()));

    addIntermediateNodes(pts, &end, currNode);

    while (*currNode != start) {
//        pts.push_back(Point(currNode->getRow(), currNode->getCol()));
        addIntermediateNodes(pts, currNode, currNode->getParent());
        currNode = currNode->getParent();
//        currNode = *currNode_ptr;
    }

//    pts.push_back(Point(currNode->getRow(), currNode->getCol()));

//    qDebug() << "curr node row is " << currNode->getRow();
//    qDebug() << "curr node col is " << currNode->getCol();
    return pts;
}
