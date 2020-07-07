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

    // This outputs 1 if input 1 < input 2, -1 if input 1 > input 2, and 0 if input 1 == input 2
    // This allows us to figure out what row, col direction we are going in to get from startNode to endNode
    int rowIncrement = boolToDirectionLessThan(startNode->getRow(), endNode->getRow(), 1, -1, 0);
    int colIncrement = boolToDirectionLessThan(startNode->getCol(), endNode->getCol(), 1, -1, 0);

    return pair<int, int>(rowIncrement, colIncrement);
}

// Basically do something while going from start node (row, col) to end node (row, col)
// The function inputted in is operated on the row and columns while iterating
bool RRT::incrementFromStartToEndNode(bool (RRT::*function)(int, int, vector<Point>&), Node* startNode, Node* endNode, vector<Point> &nodes) {
    int rowIncrement, colIncrement;
    tie(rowIncrement, colIncrement) = getIncrements(startNode, endNode);

    int col = startNode->getCol();
    int row = startNode->getRow();

    while (col != endNode->getCol()+colIncrement || row != endNode->getRow()+rowIncrement) {
        if ((this->*function)(row, col, nodes) == true) {
            return true;
        }
        col += colIncrement;
        row += rowIncrement;
    }

    return false;
}

bool RRT::addNode(int row, int col, vector<Point> &nodes) {
    nodes.push_back(Point(row, col));
    return false;
}

bool RRT::checkObstacleFunc(int row, int col, vector<Point> &nodes) {
    if (occGrid.grid[row][col] == 1) {
        return true;
    }
    return false;
}

vector<Point> RRT::findPath() {
    Node* currNode = &start;
    Node* closestNode;
    nodes.push_back(currNode);

    int rrtDist = 2;
    int margin = 0;

    srand(time(NULL)); // randomly seed random

    while (dist(end, currNode->getRow(), currNode->getCol()) > rrtDist + margin) {

        pair<int, int> currCoord = getRandomCoordinates();
        int randRow = currCoord.first;
        int randCol = currCoord.second;

        if (occGrid.grid[randRow][randCol] == 0) {
            Node* closestNode = findClosestNode(randRow, randCol);

            int rowDirection = randRow - closestNode->getRow();
            int colDirection = randCol - closestNode->getCol();

            int rrtDistRow = boolToDirection(rowDirection > 0);
            int rrtDistCol = boolToDirection(colDirection > 0);

            if (abs(rowDirection) > abs(colDirection)) {
                rrtDistCol = 0;
            }
            else if (abs(rowDirection) < abs(colDirection)) {
                rrtDistRow = 0;
            }

            Node* newNode = new Node(closestNode->getRow() + rrtDistRow*rrtDist, closestNode->getCol() + rrtDistCol*rrtDist);

            // Make sure that this a new point
            if (rowDirection == 0 && colDirection == 0) continue;

            // Make sure we are within the bounds of the occ grid
            if (!occGrid.isWithinBounds(newNode->getRow(), newNode->getCol())) {
                continue;
            }

            vector<Point> getRidOfThis;
            bool isObstacle = incrementFromStartToEndNode(&RRT::checkObstacleFunc, closestNode, newNode, getRidOfThis);

            if (!isObstacle) {
                newNode->parent = closestNode;
                currNode = newNode;
                nodes.push_back(newNode);
            }
        }
    }

    vector<Point> pts;

    incrementFromStartToEndNode(&RRT::addNode, &end, currNode, pts);

    while (*currNode != start) {
        incrementFromStartToEndNode(&RRT::addNode, currNode, currNode->getParent(), pts);
        currNode = currNode->getParent();
    }

    return pts;
}
