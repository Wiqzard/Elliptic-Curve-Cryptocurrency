#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Utils.hpp"


//#include "FieldInt.hpp"


//class FieldInt;

/* 
 * An unsigned 256-bit integer, represented as eight unsigned 32-bit words in little endian.
 */

class Uint256 {
    
    public:
    static constexpr int NUM_WORDS = 8;
    
    /*---- Fields ----*/
    
    // The mutable words representing this number in little endian, conceptually like this:
    // actualValue = value[0] << 0 | value[1] << 32 | ... | value[7] << 224.
    std::uint32_t value[NUM_WORDS];
    std::vector<std::uint32_t> value1;
    
    
    /*---- Constructors ----*/
    
    // Constructs a Uint256 initialized to zero. Constant-time.
    explicit Uint256();
    
    // Constructs a Uint256 from the given 64-character hexadecimal string. Not constant-time.
    explicit Uint256(std::string str);
    //explicit Uint256(const char *str);
    
    
    // Constructs a Uint256 from the given 32 bytes encoded in big-endian.
    explicit Uint256(const std::uint8_t b[NUM_WORDS * 4]);
    
   
	
    //explicit Uint256(const FieldInt &val);
	
    std::vector<char> Uint256ToVec();
    std::string Uint256ToString();
	
	/*---- Arithmetic methods modulo 2^256 ----*/
	
    std::uint32_t add(const Uint256 &other, std::uint32_t enable=1);
	
    std::uint32_t subtract(const Uint256 &other, std::uint32_t enable=1);
	
	
	// Shifts this number left by 1 bit (same as multiplying by 2), modulo 2^256.
	std::uint32_t shiftLeft1();
	
	// Shifts this number right by 1 bit (same as dividing by 2 and flooring).
	void shiftRight1(std::uint32_t enable=1);
	
	
	// Computes the multiplicative inverse of this number with respect to the given modulus.
	// The modulus must be odd and coprime to this number. This number must be less than the modulus.
	// If this number is zero, the reciprocal is zero. Constant-time with respect to this value.
	void reciprocal(const Uint256 &modulus);
	
	
	/*---- Miscellaneous methods ----*/
	
	// Copies the given number into this number if enable is 1, or does nothing if enable is 0.
	void replace(const Uint256 &other, std::uint32_t enable);
	
	
	// Swaps the value of this number with the given number if enable is 1, or does nothing if enable is 0.
	void swap(Uint256 &other, std::uint32_t enable);
	
	
	// Writes this 256-bit integer as 32 bytes encoded in big endian to the given array.
	// Constant-time with respect to this value.
	void getBigEndianBytes(std::uint8_t b[NUM_WORDS * 4]) const;
	
	
	/*---- Equality/inequality operators ----*/
	
	bool operator==(const Uint256 &other) const;
	
	bool operator!=(const Uint256 &other) const;
	
	bool operator<(const Uint256 &other) const;
	
	bool operator<=(const Uint256 &other) const;
	
	bool operator>(const Uint256 &other) const;

	bool operator>=(const Uint256 &other) const;
	
	
	
	/*---- Class constants ----*/
	
	static const Uint256 ZERO;
	static const Uint256 ONE;
	
};



