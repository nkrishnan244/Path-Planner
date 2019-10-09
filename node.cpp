#include "node.h"

Node::Node(int x_pos, int y_pos, int prev_g, float h_val) 
: x(x_pos), y(y_pos), g(prev_g + 1), h(h_val)
{
}

int Node::get_x() const {
    return x;
}

int Node::get_y() const {
    return y;
}

int Node::get_g() const {
    return g;
}

float Node::get_h() const {
    return h;
}

void Node::set_h(float val) {
    h = val; 
}

Node* Node::get_parent() {
    return parent; 
}

void Node::set_parent(Node &parent_node) {
    this->parent = &parent_node; 
}

bool Node::operator==(const Node& rhs)
{
    return (this->get_x() == rhs.get_x()) && (this->get_y() == rhs.get_y());
}

bool Node::operator!=(const Node& rhs)
{
    return !(*this == rhs);
}

// bool Node::operator<(const Node& rhs) { 
//   return this->get_h() > rhs.get_h();
// }