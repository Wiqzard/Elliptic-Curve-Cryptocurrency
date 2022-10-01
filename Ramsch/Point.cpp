#include <iostream>
#include "common.h"
#include "Point.h"


Point::Point(bool b) {
    this->identity = b;
}


Point::Point(Element x, Element y):identity(false) {
    this->x = x;
    this->y = y;
}


Point Point::operator =(const Point& P) {  // ASSIGNS POINT OBJECT TO POINT OBJECT
    if(P.identity)
        {
        this->identity = true;
        return *this;
        }

    this->identity=P.identity;
    this->x = P.x;
    this->y = P.y;
    return *this;
}


bool Point::operator ==(const Point& P) const {
    return (this->identity && P.identity) ||
                            (!this->identity && !P.identity && this->x == P.x && this->y == P.y);
}


void Point::print() {
    this->identity ? std::cout << "infinity "<< std::endl :std::cout << "[" << this->x << ","
    << this->y << "]" << std::endl;
}
