#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "Sha256Hash.hpp"
#include "Utils.hpp"
#include <stdint.h>
#include <stdio.h>

using std::uint8_t;   // equals C unsigned char, 0-255
using std::size_t;

Sha256Hash::Sha256Hash(){}
Sha256Hash::Sha256Hash(const uint8_t hash[HASH_LEN], size_t len) {
    assert(hash != nullptr && len == HASH_LEN);
    std::memcpy(value, hash, sizeof(value));
}

Sha256Hash::Sha256Hash(std::string str) :
            value() {
    assert(str.length() == HASH_LEN * 2);
    for (int i = 0; i < HASH_LEN * 2; i++) {
        int digit = Utils::parseHexDigit(str[HASH_LEN * 2 - 1 - i]);
        assert(digit != -1);
        value[i >> 1] |= digit << ((i & 1) << 2);
    }
}

Sha256Hash::Sha256Hash(const char *str) :
            value() {
    assert(str != nullptr && std::strlen(str) == HASH_LEN * 2);
    for (int i = 0; i < HASH_LEN * 2; i++) { //For every char in hex string:
        int digit = Utils::parseHexDigit(str[HASH_LEN * 2 - 1 - i]); //hex char to int from behind
        assert(digit != -1);
        value[i >> 1] |= digit << ((i & 1) << 2);
    }
                // >> 1 : Divides i by 2,
                // |= : val = val | digi..
                // | :  bitwise or
}



std::vector<char> Sha256Hash::Sha256HashToVec(){
    std::vector<char> vec;
    for( int i = 0; i < 32; ++i ){
        char const byte = value[i];
        vec.insert(vec.begin(), Utils::hex_chars[ ( byte & 0x0F ) >> 0 ]);
        vec.insert(vec.begin(), Utils::hex_chars[ ( byte & 0xF0 ) >> 4 ]);
    }
    return vec;
}


std::string Sha256Hash::Sha256HashToString(){
    std::vector<char> vec = Sha256HashToVec();
    std::string s(vec.begin(), vec.end());
   
    return s;
}

//std::string Sha256Hash::Sha256HashToString(){
//    std::stringstream stream;
//    for (int i = 0; i < HASH_LEN; i++){
//        stream<< std::hex << value[i];
//        std::cout << value[i];
////        std::cout << stream.str() << std::endl;
//    }
//    return stream.str();
//}


bool Sha256Hash::operator==(const Sha256Hash &other) const {
    int diff = 0;
    for (int i = 0; i < HASH_LEN; i++)
        diff |= value[i] ^ other.value[i];
    return diff == 0;
}


bool Sha256Hash::operator!=(const Sha256Hash &other) const {
    return !(*this == other);
}
