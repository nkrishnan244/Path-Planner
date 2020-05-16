#include "astar.h"

using namespace std;

Astar::Astar(Node startNode, Node endNode, OccupancyGrid &occ)
: Dijkstra(startNode, endNode, occ)
{
}

Astar::~Astar()
{
}

float calculateHeuristic(float row, float col, Node end) {
    return sqrt(pow(row - end.get_row(), 2) + pow(col - end.get_col(), 2));
}

void Astar::addNodeToQueue(int row, int col, priority_queue<Node> &pq, Node* parentNodePtr) {
    if (occGrid.grid.at(row).at(col) == 0) {
        float h = calculateHeuristic(row, col, end);
        int g = parentNodePtr->get_g() + 1;
        Node* newNodePtr = new Node(row, col, g, h);
        newNodePtr->val = g + h;
        newNodePtr->parent = parentNodePtr;
        pq.push(*newNodePtr);
    }
}

vector<vector<int>> Astar::findPath() {
    return Dijkstra::findPath();
}
