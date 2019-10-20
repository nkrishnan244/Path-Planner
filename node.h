#ifndef NODE_H
#define NODE_H

class Node {
    
    public:
        Node(int row, int col, int prev_g, float h_val);
        int get_row() const;
        int get_col() const;
        int get_g() const;
        float get_h() const; 
        void set_h(float val);
        bool operator==(const Node& rhs);
        bool operator!=(const Node& rhs);
        Node* get_parent();
        void set_parent(Node &parent_node);
        Node* parent;
        // bool operator<(const Node& rhs);

    private:
        int row;
        int col;
        int g;
        float h;
};

#endif