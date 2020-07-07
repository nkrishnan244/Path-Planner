#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point();
    Point(int initRow, int initCol);

    int getRow() const;
    int getCol() const;

    bool setRow(int inputRow);
    bool setCol(int inputCol);

    bool operator==(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;

protected:
    int row;
    int col;
};

#endif // POINT_H
