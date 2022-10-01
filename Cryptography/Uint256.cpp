#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "Uint256.hpp"

using std::uint32_t;
using std::uint64_t;
using std::uint8_t;


Uint256::Uint256() : value() {}

Uint256::Uint256(std::string str) : value(), value1(NUM_WORDS)
{
    assert(str.length() == NUM_WORDS * 8);
    for (int i = 0; i < NUM_WORDS * 8; i++)
    {
        int digit = Utils::parseHexDigit(str[NUM_WORDS * 8 - 1 - i]);
        assert(digit != -1);
        value[i >> 3] |= static_cast<uint32_t>(digit) << ((i & 7) << 2);
        value1[i >> 3] |= static_cast<uint32_t>(digit) << ((i & 7) << 2);
    }
}

Uint256::Uint256(const uint8_t b[NUM_WORDS * 4]) : value(), value1(NUM_WORDS)
{
    assert(b != nullptr);
    for (int i = 0; i < NUM_WORDS * 4; i++){
        value[i >> 2] |= static_cast<uint32_t>(b[NUM_WORDS * 4 - 1 - i]) << ((i & 3) << 3);
        value1[i >> 2] |= static_cast<uint32_t>(b[NUM_WORDS * 4 - 1 - i]) << ((i & 3) << 3);
    }
}

std::vector<char> Uint256::Uint256ToVec(){
    std::vector<char> vec;
    uint8_t arr[4];
    for( int i = 0; i < 8; ++i ){
        Utils::storeBigUint32(value[i], arr);
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[3] & 0x0F ) >> 0 ]);
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[3] & 0xF0 ) >> 4 ]);
        
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[2] & 0x0F ) >> 0 ]);
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[2] & 0xF0 ) >> 4 ]);
        
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[1] & 0x0F ) >> 0 ]);
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[1] & 0xF0 ) >> 4 ]);
        
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[0] & 0x0F ) >> 0 ]);
        vec.insert(vec.begin(), Utils::hex_chars[ ( arr[0] & 0xF0 ) >> 4 ]);
    }
    return vec;
}
std::string Uint256::Uint256ToString(){
    std::vector<char> uintvec = this->Uint256ToVec();
    std::string s(uintvec.begin(), uintvec.end());
    return s;
}

//Uint256::Uint256(const FieldInt &val)
//{
//    std::memcpy(this->value, val.value, sizeof(value));
//}

uint32_t Uint256::add(const Uint256 &other, uint32_t enable)
{
    assert(&other != this && (enable >> 1) == 0);
    uint32_t mask = -enable;
    uint32_t carry = 0;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        uint64_t sum = static_cast<uint64_t>(value[i]) + (other.value[i] & mask) + carry;
        value[i] = static_cast<uint32_t>(sum);
        carry = static_cast<uint32_t>(sum >> 32);
        assert((carry >> 1) == 0);
    }
    return carry;
}

uint32_t Uint256::subtract(const Uint256 &other, uint32_t enable)
{
    assert(&other != this && (enable >> 1) == 0);

    uint32_t mask = -enable;
    uint32_t borrow = 0;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        uint64_t diff = static_cast<uint64_t>(value[i]) - (other.value[i] & mask) - borrow;
        value[i] = static_cast<uint32_t>(diff);
        borrow = -static_cast<uint32_t>(diff >> 32);
        assert((borrow >> 1) == 0);
    }
    return borrow;
}

uint32_t Uint256::shiftLeft1()
{

    uint32_t prev = 0;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        uint32_t cur = value[i];
        value[i] = (0U + cur) << 1 | prev >> 31;
        prev = cur;
    }
    return prev >> 31;
}

void Uint256::shiftRight1(uint32_t enable)
{
    assert((enable >> 1) == 0);

    uint32_t mask = -enable;
    uint32_t cur = value[0];
    for (int i = 0; i < NUM_WORDS - 1; i++)
    {
        uint32_t next = value[i + 1];
        value[i] = ((cur >> 1 | (0U + next) << 31) & mask) | (cur & ~mask);
        cur = next;
    }
    value[NUM_WORDS - 1] = ((cur >> 1) & mask) | (cur & ~mask);
}

void Uint256::reciprocal(const Uint256 &modulus)
{
    // Extended binary GCD algorithm
    assert(&modulus != this && (modulus.value[0] & 1) == 1 && modulus > ONE && *this < modulus);
    Uint256 x = modulus;
    Uint256 y = *this;
    Uint256 a = ZERO;
    Uint256 b = ONE;
    Uint256 halfModulus = modulus;
    halfModulus.add(ONE);
    halfModulus.shiftRight1();

    // Loop invariant: x = a*this mod modulus, and y = b*this mod modulus
    for (int i = 0; i < NUM_WORDS * 32 * 2; i++)
    {

        // Try to reduce a trailing zero of y. Pseudocode:
        // if (y % 2 == 0) {
        //     y /= 2
        //     b = b % 2 == 0 ? b / 2 : modulus - (modulus - b) / 2
        // }
        assert((x.value[0] & 1) == 1);
        uint32_t yEven = (y.value[0] & 1) ^ 1;
        uint32_t bOdd = b.value[0] & 1;
        y.shiftRight1(yEven);
        b.shiftRight1(yEven);
        b.add(halfModulus, yEven & bOdd);
        // If allowed, try to swap so that y >= x and then do y -= x. Pseudocode:
        // if (y % 2 == 1) {
        //     if (x > y) {
        //         x, y = y, x
        //         a, b = b, a
        //     }
        //     y -= x
        //     b -= a
        //     b %= modulus
        // }
        uint32_t enable = y.value[0] & 1;
        uint32_t doswap = enable & static_cast<uint32_t>(x > y);
        x.swap(y, doswap);
        y.subtract(x, enable);
        a.swap(b, doswap);
        uint32_t borrow = b.subtract(a, enable);
        b.add(modulus, borrow);
    }
    assert((x == ONE) | (x == modulus)); // Either gcd(this, modulus) = 1 or this = 0
    this->replace(a, static_cast<uint32_t>(*this != ZERO));
}

void Uint256::replace(const Uint256 &other, uint32_t enable)
{
    assert((enable >> 1) == 0);

    uint32_t mask = -enable;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        value[i] = (other.value[i] & mask) | (value[i] & ~mask);
    }
}

void Uint256::swap(Uint256 &other, uint32_t enable)
{
    assert((enable >> 1) == 0);
    uint32_t mask = -enable;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        uint32_t x = this->value[i];
        uint32_t y = other.value[i];
        this->value[i] = (y & mask) | (x & ~mask);
        other.value[i] = (x & mask) | (y & ~mask);
    }
}

void Uint256::getBigEndianBytes(uint8_t b[NUM_WORDS * 4]) const
{
    assert(b != nullptr);
    for (int i = 0; i < NUM_WORDS; i++)
        Utils::storeBigUint32(value[i], &b[(NUM_WORDS - 1 - i) * 4]);
}

bool Uint256::operator==(const Uint256 &other) const
{
    uint32_t diff = 0;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        diff |= value[i] ^ other.value[i];
    }
    return diff == 0;
}

bool Uint256::operator!=(const Uint256 &other) const
{
    return !(*this == other);
}

bool Uint256::operator<(const Uint256 &other) const
{
    bool result = false;
    for (int i = 0; i < NUM_WORDS; i++)
    {
        bool eq = value[i] == other.value[i];
        result = (eq & result) | (!eq & (value[i] < other.value[i]));
    }
    return result;
}

bool Uint256::operator<=(const Uint256 &other) const
{
    return !(other < *this);
}

bool Uint256::operator>(const Uint256 &other) const
{
    return other < *this;
}

bool Uint256::operator>=(const Uint256 &other) const
{
    return !(*this < other);
}

// Static initializers
const Uint256 Uint256::ZERO;
const Uint256 Uint256::ONE("0000000000000000000000000000000000000000000000000000000000000001");
