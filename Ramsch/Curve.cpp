#include <iostream>
#include "common.h"
#include "Curve.h"

Curve::Curve() {
    Integer p;
    do {
        std::cout<< "Enter the Prime modulo p :";
        std::cin>>p;
        this->FField = new ZP(p);   //REQUESTS MEMORY ALLOCATION ON HEAP
        std::cout << "Enter A : ";
        this->FField->read(std::cin,this->A);  //To access members of a structure through a pointer, use the arrow operator.
        std::cout << "Enter B : ";
        this->FField->read(std::cin,this->B);
        std::cout<< std::endl;
        }
    while (this->isZeroDiscriminant());
    }


Curve::~Curve() {   // DESTRUCTOR FREES PREVIOUSLY ALLOCATED MEMORY
    free(FField);
}

Curve::Curve(Integer primeField, Integer A, Integer B)
    {
    this->FField = new ZP(primeField);
    this->FField->init(this->A,A);
    this->FField->init(this->B,B);
// DOUBLE ARROWS ASIGNS THE ATTRIBUTE FFIELD OF THIS OBJECT THE VALUE init(..)
    if (this->isZeroDiscriminant()){
        std::cerr << "[!] Curve not defined, disriminant is Zero" << std::endl;
        std::cout << "[+ INFO ] Field : Z/" << primeField << "Z" << std::endl;
        std::cout << "[+ INFO ] A : " << A << " ; B : " << B << std::endl;
        abort();
        }
    }
void Curve::print() {
    std::cout << " Field : Z/" << this->FField->residu() << "Z" << std::endl;
    std::cout << " A :" ;
    this->FField->write(std::cout,this->A);
    std::cout << ", B :";
    this->FField->write(std::cout,this->B);
    std::cout<< std::endl;
}

bool Curve::isZeroDiscriminant(){
    Element Acube, Bsquare;
    this->FField->mul(Acube, this->A, this->A);
    this->FField->mulin(Acube, this->A);
    this->FField->mulin(Acube, Integer("4"));

    this->FField->mul(Bsquare, this->B, this->B);
    this->FField->mulin(Bsquare, Integer("27"));

    this->FField->addin(Acube, Bsquare);
    return this->FField->isZero(Acube);
}
