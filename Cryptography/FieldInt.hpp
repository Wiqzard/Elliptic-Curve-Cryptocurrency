#pragma once

#include <cstdint>
#include "Uint256.hpp"



 // Unsigned 256-bit integer modulo a specific prime number
class FieldInt final : public Uint256 {

public:
    
    using Uint256::NUM_WORDS;
    
    /*---- Fields ----*/
    
    using Uint256::value;
    
    
    
    /*---- Constructors ----*/
    
    // Constructs a FieldInt from the given 64-character hexadecimal string.
    explicit FieldInt(const char *str);
    
    
    // Constructs a FieldInt from the given Uint256, reducing it as necessary.
    explicit FieldInt(const Uint256 &val);
    
    
    /*---- Arithmetic methods ----*/
    
    public: void add(const FieldInt &other);
    
    public: void subtract(const FieldInt &other);
    
    
    public: void multiply2();
    
    
    // Squares this number, modulo the prime. Constant-time with respect to this value.
    public: void square();
    
    
    // Multiplies the given number into this number, modulo the prime. Constant-time with respect to both values.
    public: void multiply(const FieldInt &other);
    
    
    // Computes the multiplicative inverse of this number with respect to the modulus.
    // If this number is zero, the reciprocal is zero. Constant-time with respect to this value.
    public: void reciprocal();
    
    
    /*---- Miscellaneous methods ----*/
    
    public: void replace(const FieldInt &other, std::uint32_t enable);
    
    public: using Uint256::getBigEndianBytes;
    
    
    /*---- Equality and inequality operators ----*/
    
    public: bool operator==(const FieldInt &other) const;
    
    public: bool operator!=(const FieldInt &other) const;
    
    public: bool operator<(const FieldInt &other) const;
    
    public: bool operator<=(const FieldInt &other) const;
    
    public: bool operator>(const FieldInt &other) const;
    
    public: bool operator>=(const FieldInt &other) const;
    
    
    private: bool operator<(const Uint256 &other) const;
    
    private: bool operator>=(const Uint256 &other) const;
    
    
    
    /*---- Class constants ----*/
    
    private: static const Uint256 MODULUS;  // Prime number
    
};
