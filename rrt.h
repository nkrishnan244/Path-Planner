#ifndef RRT_H
#define RRT_H

#include "planner.h"
#include "node.h"
#include "occupancy_grid.h"
#include <vector>
#include <stdlib.h>
#include "helper_functions.h"
#include <functional>

class RRT : public Planner
{
    public:
        RRT(Node startNode, Node endNode, OccupancyGrid &occ);
        virtual ~RRT() {}
        virtual vector<Point> findPath();
        pair<int, int> getRandomCoordinates();
        Node* findClosestNode(int randRow, int randCol);
        bool checkObstacle(Node* startNode, Node* finalNode);
        bool addIntermediateNodes(vector<Point> &nodes, Node* closestNode, Node* currNode);

        bool checkObstacleFunc(int row, int col, vector<Point> &nodes);

        bool incrementFromStartToEndNode(bool (RRT::*function)(int, int, vector<Point>&), Node* startNode, Node* endNode, vector<Point> &nodes);
//        bool incrementFromStarttoEndNode(std::function<bool&(int, int, vector<Point>)> funct, Node* startNode, Node* endNode, vector<Point> &nodes);

        bool addNode(int row, int col, vector<Point> &nodes);

//        int rrt_dist;

//        void set_rrt_dist(int val) {rrt_dist=val;}
        vector<Node*> nodes;
};

#endif // RRT_H
