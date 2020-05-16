#include <iostream>
#include "dijkstra.h"
#include <math.h>
#include <unordered_set>
#include "mainwindow.h"

using namespace std;

Dijkstra::Dijkstra(Node startNode, Node endNode, OccupancyGrid &occ)
: Planner(startNode, endNode, occ)
{

}

void Dijkstra::addNodeToQueue(unsigned int row, unsigned int col, priority_queue<Node> &pq, Node* parentNodePtr) {
    if (occGrid.grid.at(row).at(col) == 0) {
        int g = parentNodePtr->get_g() + 1;
        Node* newNodePtr = new Node(row, col, g);
        newNodePtr->val = g;
        newNodePtr->parent = parentNodePtr;
        pq.push(*newNodePtr);
    }

}

namespace std
{
template<>
    struct hash<Node>
    {
        size_t
        operator()(const Node &node) const
        {
            return hash<int>()(node.get_row()) ^ hash<int>()(node.get_col());
        }
    };
}

vector<vector<int>> Dijkstra::findPath() {
    unordered_set<Node> seenNodes;
    priority_queue<Node> pqueue;
    Node* currNodePtr = &start;

    while (*currNodePtr != end) {
        seenNodes.insert(*currNodePtr);
        int currRow = currNodePtr->get_row();
        int currCol = currNodePtr->get_col();

        // Check down
        if (currRow < occGrid.grid.size() - 1) {
            addNodeToQueue(currRow + 1, currCol, pqueue, currNodePtr);
        }

        // Check up
        if (currRow > 0) {
            addNodeToQueue(currRow - 1, currCol, pqueue, currNodePtr);
        }

        // Check right
        if (currCol < occGrid.grid.size() - 1) {
            addNodeToQueue(currRow, currCol + 1, pqueue, currNodePtr);
        }

        // Check left
        if (currCol > 0) {
            addNodeToQueue(currRow, currCol - 1, pqueue, currNodePtr);
        }

        // Up right
        if (currRow > 0 && currCol < occGrid.grid.size() - 1) {
            addNodeToQueue(currRow - 1, currCol + 1, pqueue, currNodePtr);
        }

        // Up left
        if (currRow > 0 && currCol > 0) {
            addNodeToQueue(currRow - 1, currCol - 1, pqueue, currNodePtr);
        }

        // Bottom Right
        if (currRow < occGrid.grid.size() - 1 && currCol < occGrid.grid.size() - 1) {
            addNodeToQueue(currRow + 1, currCol + 1, pqueue, currNodePtr);
        }

        // Bottom Left
        if (currRow < occGrid.grid.size() - 1 && currCol > 0) {
            addNodeToQueue(currRow + 1, currCol - 1, pqueue, currNodePtr);
        }

        Node nextNode = pqueue.top();
        pqueue.pop();

        while (seenNodes.find(nextNode) != seenNodes.end()) {
            nextNode = pqueue.top();
            pqueue.pop();
        }

        currNodePtr = new Node(nextNode.get_row(), nextNode.get_col(), nextNode.get_g(), nextNode.get_h());
        currNodePtr->parent = nextNode.parent;
    }

    vector<int> finalRows;
    vector<int> finalCols;

    finalRows.push_back(currNodePtr->get_row());
    finalCols.push_back(currNodePtr->get_col());

    while (*currNodePtr != start) {
        currNodePtr = currNodePtr->get_parent();
        finalRows.push_back(currNodePtr->get_row());
        finalCols.push_back(currNodePtr->get_col());
    }

    return {finalRows, finalCols};
}
