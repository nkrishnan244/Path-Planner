#include "astar.h"
#include <QDebug>
//#include "helper_functions.cpp"

using namespace std;

Astar::Astar(Node startNode, Node endNode, OccupancyGrid &occ)
: Dijkstra(startNode, endNode, occ)
{
}

Astar::~Astar()
{
}

void Astar::setNodeValue(Node &newNode) {
    double h = dist(end, newNode.getRow(), newNode.getCol());
    newNode.setVal(newNode.getG() + h);
}
