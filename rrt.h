#ifndef RRT_H
#define RRT_H

#include "planner.h"
#include "node.h"
#include "occupancy_grid.h"
#include <vector>


class RRT : public Planner
{
    public:
        RRT(Node startNode, Node endNode, OccupancyGrid &occ);
        virtual ~RRT() {}
        virtual vector<Point> findPath();
        pair<int, int> getRandomCoordinates();
        Node* findClosestNode(int randRow, int randCol);
        bool checkObstacle(Node* startNode, Node* finalNode);
        void addIntermediateNodes(vector<Point> &nodes, Node* closestNode, Node* currNode);

//        int rrt_dist;

//        void set_rrt_dist(int val) {rrt_dist=val;}
        vector<Node*> nodes;
};

#endif // RRT_H
