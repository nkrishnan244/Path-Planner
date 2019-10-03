#include <iostream>
#include "planner.h"
#include "node.h"

using namespace std;

int main() {
    vector<vector<int>> occ; 
    int occ_size = 5;
    for (int row = 0; row < occ_size; row++) {
        vector<int> curr_row {};
        for (int col = 0; col < occ_size; col++) {
            curr_row.push_back(0);
        }
        occ.push_back(curr_row);
    }

    // Node start(0, 0);
    // Node end(4, 4);


    cout << "Hello World";
    return 0;
}