#ifndef NODE_H
#define NODE_H

// #include "dijkstra.cpp"

class Node {

    public:
        Node();
        Node(int row_, int col_, int g_val);
        Node(int row_, int col_, int g_val, float h_val);
        int get_row() const;
        int get_col() const;
        int get_g() const;
        float get_h() const;
        void set_h(float val);
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const;
        bool operator<(const Node& rhs) const;
        Node* get_parent();
        void set_parent(Node &parent_node);
        Node* parent;
        // bool operator<(const Node& rhs);

        float val;

    private:
        int row;
        int col;
        int g;
        float h;
};

#endif
