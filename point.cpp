#include "point.h"
#include "QDebug"

Point::Point() {
}

Point::Point(int initRow, int initCol)
    : row(initRow), col(initCol) {
}

int Point::getRow() const {
    return row;
}

int Point::getCol() const {
    return col;
}

bool Point::setRow(int inputRow) {
//    if (inputRow < 0) {
//        qDebug() << "The row " << inputRow << " is invalid";
//        return false;
//    }
    row = inputRow;
    return true;
}

bool Point::setCol(int inputCol) {
//    if (inputCol < 0) {
//        qDebug() << "The col " << inputCol << " is invalid";
//        return false;
//    }
    col = inputCol;
    return true;
}

bool Point::operator==(const Point& rhs) const
{
    return (this->getRow() == rhs.getRow()) && (this->getCol() == rhs.getCol());
}

bool Point::operator!=(const Point& rhs) const
{
    return !(*this == rhs);
}
