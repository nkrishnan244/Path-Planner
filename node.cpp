#include "node.h"

Node::Node()
{

}

Node::Node(int row_, int col_, int g_)
: row(row_), col(col_), g(g_)
{
}

Node::Node(int row_, int col_, int g_, float h_)
: row(row_), col(col_), g(g_), h(h_)
{
}

int Node::get_row() const {
    return row;
}

int Node::get_col() const {
    return col;
}

void Node::set_row(int newRow) {
    row = newRow;
}

void Node::set_col(int newCol) {
    col = newCol;
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

void Node::set_parent(Node &parentNode) {
    this->parent = &parentNode;
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
