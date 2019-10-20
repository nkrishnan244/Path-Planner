#include "node.h"

Node::Node(int row_, int col_, int prev_g, float h_val) 
: row(row_), col(col_), g(prev_g + 1), h(h_val)
{
}

int Node::get_row() const {
    return row;
}

int Node::get_col() const {
    return col;
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
    return (this->get_row() == rhs.get_row()) && (this->get_col() == rhs.get_col());
}

bool Node::operator!=(const Node& rhs)
{
    return !(*this == rhs);
}

// bool Node::operator<(const Node& rhs) { 
//   return this->get_h() > rhs.get_h();
// }