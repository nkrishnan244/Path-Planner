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

float dist(Node node, int row, int col) {
    return sqrt(pow(node.get_row() - row, 2) + pow(node.get_col() - col, 2));
}

Node RRT::findClosestNode(int randRow, int randCol) {
//    float min_dist = numeric_limits<float>::max();
//    Node closestNode;
//    for (Node node:nodes) {
//        float curr_dist = dist(node, randRow, randCol);
//        if (curr_dist < min_dist) {
//            closestNode = node;
//            min_dist = curr_dist;
//        }
//    }
//    return closestNode;
    return Node();
}

bool RRT::checkObstacle(Node* startNode, Node* endNode) {
    int min_row = min(startNode->get_row(), endNode->get_row());
    int max_row = max(startNode->get_row(), endNode->get_row());
    int min_col = min(startNode->get_col(), endNode->get_col());
    int max_col = max(startNode->get_col(), endNode->get_col());
    for (int row = min_row; row <= max_row; row++) {
        for (int col = min_col; col <= max_col; col++) {
            if (occGrid.grid[row][col] == 1)
                return true;
        }
    }
    return false;
}

vector<vector<int>> RRT::findPath() {
    Node* currNode = &start;
    Node* closestNode;
    nodes.push_back(currNode);
    vector<int> row;
    vector<int> col;
    int rrtDist = 8;
    int margin = 0;
    srand(time(NULL));
    while (dist(end, currNode->get_row(), currNode->get_col()) > rrtDist + margin) {
        pair<int, int> currCoord = getRandomCoordinates();
        int randRow = currCoord.first;
        int randCol = currCoord.second;
//        qDebug() << "RANDOM ROW IS " << randRow;
//        qDebug() << "RANDOM COL IS " << randCol;
        if (occGrid.grid[randRow][randCol] == 0) {
//            Node closestNode = find_closestNode(randRow, randCol);
            float minDist = numeric_limits<float>::max();
            for (Node* node:nodes) {
                float currDist = dist(*node, randRow, randCol);
                if (currDist < minDist) {
                    closestNode = node;
                    minDist = currDist;
                }
            }
            Node* newNode = new Node();
            int rowDirection = randRow - closestNode->get_row();
            int colDirection = randCol - closestNode->get_col();
            if (abs(rowDirection) > abs(colDirection)) {
                if (rowDirection < 0) {
                    newNode->set_row(closestNode->get_row() - rrtDist);
                } else {
                    newNode->set_row(closestNode->get_row() + rrtDist);
                }
                newNode->set_col(closestNode->get_col());
            } else if (abs(colDirection) > abs(rowDirection)) {
                if (colDirection < 0) {
                    newNode->set_col(closestNode->get_col() - rrtDist);
                } else {
                    newNode->set_col(closestNode->get_col() + rrtDist);
                }
                newNode->set_row(closestNode->get_row());
            } else {
                if (rowDirection == 0) continue;
                if (rowDirection > 0) {
                    newNode->set_row(closestNode->get_row() + rrtDist);
                    newNode->set_col(closestNode->get_col() + rrtDist);
                } else {
                    newNode->set_row(closestNode->get_row() - rrtDist);
                    newNode->set_col(closestNode->get_col() - rrtDist);
                }
            }
            if (newNode->get_row() >= occGrid.grid.size() || newNode->get_col() >= occGrid.grid.size())
                continue;
            if (newNode->get_row() < 0 || newNode->get_col() < 0)
                continue;

            bool isObstacle = checkObstacle(closestNode, newNode);

            if (!isObstacle) {
                newNode->parent = closestNode;
                currNode = newNode;
                nodes.push_back(newNode);
//                qDebug() << "new node col is " << newNode->get_col();
//                qDebug() << "new node row is " << newNode->get_row();
            }
        }
    }

    row.push_back(end.get_row());
    col.push_back(end.get_col());

    while (*currNode != start) {
        row.push_back(currNode->get_row());
        col.push_back(currNode->get_col());
//        qDebug() << "curr node row is " << currNode->get_row();
//        qDebug() << "curr node col is " << currNode->get_col();
        currNode = currNode->get_parent();
//        currNode = *currNode_ptr;
    }
    row.push_back(currNode->get_row());
    col.push_back(currNode->get_col());
//    qDebug() << "curr node row is " << currNode->get_row();
//    qDebug() << "curr node col is " << currNode->get_col();
    return {row, col};
}
