#include "node.h"

Node::Node()
{

}

Node::Node(int row_, int col_, int g_val)
: row(row_), col(col_), g(g_val)
{
}

Node::Node(int row_, int col_, int g_val, float h_val)
: row(row_), col(col_), g(g_val), h(h_val)
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

bool Node::operator==(const Node& rhs) const
{
    return (this->get_row() == rhs.get_row()) && (this->get_col() == rhs.get_col());
}

bool Node::operator!=(const Node& rhs) const
{
    return !(*this == rhs);
}

bool Node::operator<(const Node& rhs) const
{
  return this->val > rhs.val;
}
