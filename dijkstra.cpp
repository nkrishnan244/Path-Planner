#include <iostream>
#include "dijkstra.h"
#include <math.h>
#include <unordered_set>
#include "mainwindow.h"
#include <QDebug>

using namespace std;

Dijkstra::Dijkstra(Node startNode, Node endNode, OccupancyGrid &occ)
: Planner(startNode, endNode, occ)
{
}

void Dijkstra::setNodeValue(Node &newNode) {
    newNode.setVal(newNode.getG());
}

void Dijkstra::addNodeToQueue(int row, int col, priority_queue<Node> &pq, Node* parentNode) {
    Node newNode(row, col);
    newNode.parent = parentNode;
    double g;

    // Along a diagnal, the step is greater than 1 (sqrt of 2)
    if (row != parentNode->getRow() && col != parentNode->getCol()) {
        g = parentNode->getG() + diagonalDistance;
    }
    else {
        g = parentNode->getG() + nonDiagonalDistance;
    }
    newNode.setG(g);
    setNodeValue(newNode); // For Dijkstra, only the number of steps determines the value of a node
    pq.push(newNode);
}

// Creates a hash function that is used for marking row, col pairs as seen
struct pair_hash {
    inline size_t operator()(const pair<int, int> &v) const {
        return hash<int>()(v.first) ^ hash<int>()(v.second);
    }
};

vector<Point> Dijkstra::findPath() {

    // Creates a vector that represents the position of all neighbors relative to current node
    vector<pair<int, int>> neighborIncVec{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {1, 0}};

    unordered_set<pair<int, int>, pair_hash> seenNodes;
    priority_queue<Node> pqueue;
    Node* currNode = new Node(start);

    auto startTime = chrono::high_resolution_clock::now();

    while (*currNode != end) {

        // Check to make sure that solution is not taking too long to find
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration<double>(endTime - startTime);
        if (duration.count() > maxSolveTime) {
            return {};
        }

        int currRow = currNode->getRow();
        int currCol = currNode->getCol();
        seenNodes.insert({currRow, currCol});

        // adds all the neighbors to the priority queue
        for (pair<int, int> neighborInc : neighborIncVec) {
            if (occGrid.isValidPosition(currRow + neighborInc.first, currCol + neighborInc.second)) {
                addNodeToQueue(currRow + neighborInc.first, currCol + neighborInc.second, pqueue, currNode);
            }
        }

        // Only look at the next node if it has not been seen
        while (seenNodes.find({pqueue.top().getRow(), pqueue.top().getCol()}) != seenNodes.end()) {
            pqueue.pop();
        }

        currNode = new Node(pqueue.top());
    }

    // iterate backwards from the end to get the actual path
    vector<Point> pts;
    pts.push_back(Point(currNode->getRow(), currNode->getCol()));

    while (*currNode != start) {
        currNode = currNode->getParent(); // when it is an r-value, de-reference with a *
        pts.push_back(Point(currNode->getRow(), currNode->getCol()));
    }

    return pts;
}
