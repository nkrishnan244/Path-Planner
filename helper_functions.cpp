#include "node.h"
#include "math.h"

double dist(Node node, int row, int col) {
    return sqrt(pow(node.getRow() - row, 2) + pow(node.getCol() - col, 2));
}

double dist(Node* node, int row, int col) {
    return sqrt(pow(node->getRow() - row, 2) + pow(node->getCol() - col, 2));
}

// Essentially takes a boolean and converts it to 1 if it is true and -1 if it is false
// Useful if one condition leads to a row/col increment of 1, while the opposite leads to an increment of -1
int boolToDirection(bool boolVal, int trueVal=1, int falseVal=-1) {
    if (boolVal) {
        return trueVal;
    } else {
        return falseVal;
    }
}

int boolToDirectionLessThan(int val1, int val2, int trueVal=1, int falseVal=-1, int equalVal=0) {
    if (val1 < val2) {
        return trueVal;
    } else if (val1 > val2) {
        return falseVal;
    } else {
        return equalVal;
    }
}
