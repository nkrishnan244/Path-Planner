#ifndef NODE_H
#define NODE_H

#include "point.h"

class Node : public Point {

    public:
        Node();
        Node(int initRow, int initCol, double g_val=0.0);
        Node(int initRow, int initCol, double initG, double initH);

        double getG() const;
        double getH() const;
        Node* getParent();

        bool setG(double val);
        bool setH(double val);
        bool setVal(double val);
        void setParent(Node &parent_node);

        bool operator<(const Node& rhs) const;

        // Make these private at some point
        Node* parent;
        double val;

    private:
        double g;
        double h;
};

#endif
