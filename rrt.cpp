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

void RRT::addIntermediateNodes(vector<Point> &nodes, Node* childNode, Node* parentNode) {

    // Horizontal
    if (parentNode->getRow() == childNode->getRow()) {

        // right
        if (parentNode->getCol() > childNode->getCol()) {
            for (int col = childNode->getCol(); col <= parentNode->getCol(); col++) {
                nodes.push_back(Point(parentNode->getRow(), col));
            }

        }

        // left
        else if (parentNode->getCol() < childNode->getCol()) {
            for (int col = childNode->getCol(); col >= parentNode->getCol(); col--) {
                nodes.push_back(Point(parentNode->getRow(), col));
            }
        }
    }

    // Vertical
    else if (parentNode->getCol() == childNode->getCol()) {

        // down
        if (parentNode->getRow() > childNode->getRow()) {
            for (int row = childNode->getRow(); row <= parentNode->getRow(); row++) {
                nodes.push_back(Point(row, parentNode->getCol()));
            }

        }

        // right
        else if (parentNode->getRow() < childNode->getRow()) {
            for (int row = childNode->getRow(); row >= parentNode->getRow(); row--) {
                nodes.push_back(Point(row, parentNode->getCol()));
            }

        }
    }

    // diagnal
    else {

        int col = childNode->getCol();

        // down
        if (parentNode->getRow() > childNode->getRow()) {

            // right
            if (parentNode->getCol() > childNode->getCol()) {
                for (int row = childNode->getRow(); row <= parentNode->getRow(); row++) {
                        nodes.push_back(Point(row, col));
                        col++;
                }
            }

            // left
            else if (parentNode->getCol() < childNode->getCol()) {
                for (int row = childNode->getRow(); row <= parentNode->getRow(); row++) {
                        nodes.push_back(Point(row, col));
                        col--;
                }
            }
        }

        // up
        else if (parentNode->getRow() < childNode->getRow()) {

            // right
            if (parentNode->getCol() > childNode->getCol()) {
                for (int row = childNode->getRow(); row >= parentNode->getRow(); row--) {
                        nodes.push_back(Point(row, col));
                        col++;
                }
            }

            // left
            else if (parentNode->getCol() < childNode->getCol()) {
                for (int row = childNode->getRow(); row >= parentNode->getRow(); row--) {
                        nodes.push_back(Point(row, col));
                        col--;
                }

            }

        }

    }

}

bool RRT::checkObstacle(Node* startNode, Node* endNode) {
    int min_row = min(startNode->getRow(), endNode->getRow());
    int max_row = max(startNode->getRow(), endNode->getRow());
    int min_col = min(startNode->getCol(), endNode->getCol());
    int max_col = max(startNode->getCol(), endNode->getCol());
    for (int row = min_row; row <= max_row; row++) {
        for (int col = min_col; col <= max_col; col++) {
            if (occGrid.grid[row][col] == 1)
                return true;
        }
    }
    return false;
}

vector<Point> RRT::findPath() {
    Node* currNode = &start;
    Node* closestNode;
    nodes.push_back(currNode);

    int rrtDist = 2;
    int margin = 0;
    float minDist;

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

            // Make sure that this a new point
            if (rowDirection == 0 && colDirection == 0) continue;

            // horizontal dist < vertical dist
            if (abs(rowDirection) > abs(colDirection)) {
                if (rowDirection < 0) {
                    newNode->setRow(closestNode->getRow() - rrtDist);
                } else {
                    newNode->setRow(closestNode->getRow() + rrtDist);
                }
                newNode->setCol(closestNode->getCol());
            }

            // horizontal dist > vertical dist
            else if (abs(colDirection) > abs(rowDirection)) {
                if (colDirection < 0) {
                    newNode->setCol(closestNode->getCol() - rrtDist);
                } else {
                    newNode->setCol(closestNode->getCol() + rrtDist);
                }
                newNode->setRow(closestNode->getRow());
            }

            // horizontal dist == vertical dist (need to fix this)
            else {

                // bottom right
                if (rowDirection > 0 && colDirection > 0) {
                    newNode->setRow(closestNode->getRow() + rrtDist);
                    newNode->setCol(closestNode->getCol() + rrtDist);
                }

                // top left
                else if (rowDirection < 0 && colDirection < 0) {
                    newNode->setRow(closestNode->getRow() - rrtDist);
                    newNode->setCol(closestNode->getCol() - rrtDist);
                }

                // bottom left
                else if (rowDirection > 0 && colDirection < 0) {
                    newNode->setRow(closestNode->getRow() + rrtDist);
                    newNode->setCol(closestNode->getCol() - rrtDist);
                }

                // top right
                else if (rowDirection < 0 && colDirection > 0) {
                    newNode->setRow(closestNode->getRow() - rrtDist);
                    newNode->setCol(closestNode->getCol() + rrtDist);
                }
            }

            // Make sure we are within the bounds of the occ grid
            if (newNode->getRow() >= occGrid.grid.size() || newNode->getCol() >= occGrid.grid.size())
                continue;
            if (newNode->getRow() < 0 || newNode->getCol() < 0)
                continue;

            bool isObstacle = checkObstacle(closestNode, newNode);

            if (!isObstacle) {

//                addIntermediateNodes(nodes, closestNode, newNode);


                newNode->parent = closestNode;
                currNode = newNode;
                qDebug() << "ROW IS " << newNode->getRow();
                qDebug() << "COL IS " << newNode->getCol();
                nodes.push_back(newNode);



//                addIntermediateNodes(nodes, closestNode, newNode);
//                qDebug() << "new node col is " << newNode->getCol();
//                qDebug() << "new node row is " << newNode->getRow();
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
