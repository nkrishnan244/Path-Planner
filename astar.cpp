#include "astar.h"
#include <QDebug>

using namespace std;

Astar::Astar(Node startNode, Node endNode, OccupancyGrid &occ)
: Dijkstra(startNode, endNode, occ)
{
}

Astar::~Astar()
{
}

double Astar::calculateHeuristic(double row, double col) {
    return sqrt(pow(row - end.getRow(), 2) + pow(col - end.getCol(), 2));
}

void Astar::setNodeValue(Node &newNode) {
    double h = calculateHeuristic(newNode.getRow(), newNode.getCol());
    newNode.setVal(newNode.getG() + h);
}
