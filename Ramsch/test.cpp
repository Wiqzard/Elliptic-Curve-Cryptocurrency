#include <iostream>

#include "common.h"
#include "Point.h"
#include "Curve.h"
#include "EllipticCurve.h"

using namespace Givaro;

//int main(int argc, char** argv)
    {
    /**
    ** y^2 = x^3 + 7
    **/
    EllipticCurve ecc(Integer("115792089237316195423570985008687907853269984665640564039457584007908834671663"),Integer("0"),Integer("7"));

    ecc.print();

    Point P(Integer("65485170049033141755572552932091555440395722142984193594072873483228125624049"), Integer("73163377763031141032501259779738441094247887834941211187427503803434828368457"));

    if (ecc.verifyPoint(P)){
        P.print();
        std::cout << "Point : verified!!!!" << std::endl;
    };

    Point P2;
    ecc._inv(P2,P);
    if (ecc.verifyPoint(P2)){
        P2.print();
        std::cout << "Inverse : verified" << std::endl;
    };

    Point R;
    R = ecc._double(R, P);
    if (ecc.verifyPoint(R)){
        R.print();
        std::cout << "double : verified" << std::endl;
    };

    Point Rp ;
    Rp = ecc._add(Rp, P , P);
    if (ecc.verifyPoint(Rp)){
        Rp.print();
        std::cout << "add : verified" << std::endl;
    };

    assert(Rp == R);

    Point R5;
    R5 = ecc._scalar(R5,P,Integer("13257"));
    if (ecc.verifyPoint(R5)){
        R5.print();
        std::cout << "scalar : verified" << std::endl;
    };

    return 0;
    }
