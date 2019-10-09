#ifndef NODE_H
#define NODE_H

class Node {
    
    public:
        Node(int x_pos, int y_pos, int prev_g, float h_val);
        int get_x() const;
        int get_y() const;
        int get_g() const;
        float get_h() const; 
        void set_h(float val);
        bool operator==(const Node& rhs);
        bool operator!=(const Node& rhs);
        Node* get_parent();
        void set_parent(Node& parent_node);
        // bool operator<(const Node& rhs);

    private:
        int x;
        int y;
        int g;
        float h;
        Node* parent;
};

#endif