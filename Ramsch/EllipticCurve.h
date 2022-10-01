#ifndef ELLIPTICCURVE_H_
#define ELLIPTICCURVE_H_
class EllipticCurve
{
private:
    typedef ZP::Element Element;
    Curve *curve;
    ZP *FField;
    Point identity;
public:
    EllipticCurve(Curve *c);
    EllipticCurve(Integer primeField, Integer A, Integer B);
    ~EllipticCurve();

    const Point& _inv(Point& Q, const Point& P);
    bool _isInv(const Point& Q, const Point& P);
    Point& _double(Point& R, const Point& P);
    Point& _add(Point&R, const Point &P, const Point& Q);
    Point& _scalar(Point& R, const Point& P,Integer k);

    bool verifyPoint(const Point& P) const;
    void print();

};
#endif
