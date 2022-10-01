#ifndef POINT_H_
#define POINT_H_


class Point
{
private:
    
    bool identity;
    Element x,y;            // TYPE DEF IN COMMON
public:

    Point() : identity(true) {}  // INITIALIZES MEMBER VARIABLE BEFORE BODY OF CONSTRUCTOR EXECUTES
    Point(bool b);
    Point(Element x, Element y);   // NOTE THE OVERLOADING
    
    Element getX() const{
        return this->x;
    };
    void setX(Element _x){
        this->x = _x;
    }
    Element getY() const{
        return this->y;
    };

    void setY(Element _y) {
        this->y = _y;
    };

    bool isIdentity() const{
        return this->identity;
    };

    void setIdentity(bool _identity){
        this->identity = _identity;
    };

    Point operator=(const Point& P);   // OPERATOR OVERLOADING
    bool operator==(const Point& P) const;
    void print();
};

#endif
