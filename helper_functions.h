#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "node.h"

extern double dist(Node node, int row, int col);
extern double dist(Node* node, int row, int col);
extern int boolToDirection(bool boolVal, int trueVal=1, int falseVal=-1);
extern int boolToDirectionLessThan(int val1, int val2, int trueVal=1, int falseVal=-1, int equalVal=0);

#endif // HELPER_FUNCTIONS_H
