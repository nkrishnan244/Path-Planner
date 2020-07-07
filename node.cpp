#include "node.h"

Node::Node()
{
}

Node::Node(int initRow, int initCol, double initG)
    : Point(initRow, initCol), g(initG)
{
}

Node::Node(int initRow, int initCol, double initG, double initH)
    : Point(initRow, initCol), g(initG), h(initH)
{
}

double Node::getG() const {
    return g;
}

double Node::getH() const {
    return h;
}

bool Node::setG(double val) {
    g = val;
    return true;
}

bool Node::setH(double val) {
    h = val;
    return true;
}

bool Node::setVal(double inputVal) {
    val = inputVal;
    return true;
}

Node* Node::getParent() {
    return parent;
}

void Node::setParent(Node &parentNode) {
    this->parent = &parentNode;
}

bool Node::operator<(const Node& rhs) const
{
  return this->val > rhs.val;
}
