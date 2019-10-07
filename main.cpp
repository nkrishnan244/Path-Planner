#include <iostream>
#include "planner.h"
#include "node.h"
#include "node.cpp" 
#include "planner.cpp"

using namespace std;

int main() {
    int occ_size = 5;
    Node start(0, 0, 0, 0);
    Node end(4, 4, 0, 0);

    vector<vector<int>> occ(occ_size); 

    vector<int> curr_row(occ_size, 0);
    for (int col = 0; col < occ_size; col++) {
        occ.at(col) = curr_row;
    }

    Planner planner(start, end, occ);
    planner.find_path();

    cout << "Hello World";
    return 0;
}