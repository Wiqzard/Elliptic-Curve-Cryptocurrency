#ifndef CURVE_H_
#define CURVE_H_


class Curve
    {
    private:
        ZP* FField;
        Element A,B;

    public:
        Curve();
        Curve(Integer primeField, Integer A, Integer B);
        ~Curve();   // DESTRUCTOR
        
        bool isZeroDiscriminant();
        ZP *getField()
            {
            return this->FField;
            };

        Element getA(){
            return this->A;
        };
        Element getB(){
            return this->B;
        };
        void print();
    };

#endif
