#include <cstring>
#include "Utils.hpp"

using std::uint8_t;


int Utils::parseHexDigit(int ch) {
    if ('0' <= ch && ch <= '9')
        return ch - '0';
    else if ('a' <= ch && ch <= 'f')
        return ch - 'a' + 10;
    else if ('A' <= ch && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}


void Utils::copyBytes(void *dest, const void *src, std::size_t count) {
    if (count > 0)
        std::memmove(dest, src, count);
}


void Utils::storeBigUint32(std::uint32_t x, uint8_t arr[4]) {
    arr[0] = static_cast<uint8_t>(x >> 24);
    arr[1] = static_cast<uint8_t>(x >> 16);
    arr[2] = static_cast<uint8_t>(x >>  8);
    arr[3] = static_cast<uint8_t>(x >>  0);
}

std::string Utils::genRandom(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEF";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}
const char *Utils::HEX_DIGITS = "0123456789abcdef";
