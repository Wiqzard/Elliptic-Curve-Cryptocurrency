#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <ctime>
#include <unistd.h>

class Utils final {
    
    public:
    static const char *HEX_DIGITS;
    
    constexpr static char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    // Returns the numerical value of a hexadecimal digit character
    // (e.g. '9' -> 9, 'a' -> 10, 'B' -> 11), or -1 if the character is invalid.
    static int parseHexDigit(int ch);
    
    
    // A safe wrapper over memmove() to avoid undefined behavior. This function can be a drop-in replacement
    // for both memcpy() and memmove(). It is useful when count is a variable number that is sometimes zero.
    // Note that src and dest can overlap.
    // The function returns immediately if count is 0. This is safer than calling memmove() with a count of 0, because
    // it would be undefined behavior if src or dest is null, or if either one is pointing to the end of an array.
    // The function is not helpful for code that calls memcpy/memmove with a known positive constant count value.
    static void copyBytes(void *dest, const void *src, std::size_t count);
    
    
    static void storeBigUint32(std::uint32_t x, std::uint8_t arr[4]);
    
    static std::string genRandom(const int len);
    
    
    Utils() = delete;  // Not instantiable
    
    typedef std::vector<std::uint8_t> Bytes;
    
    static std::vector<std::uint8_t>  asciiBytes(const char *str) {
        return std::vector<std::uint8_t> (str, str + std::strlen(str));    }
    
    static std::vector<std::uint8_t>  asciiBytes(const std::string str) {
        std::vector<char> bytes(str.begin(), str.end());
        bytes.push_back('\0');
        //std::cout << "Bytes size " << bytes.size() << std::endl;
        return std::vector<std::uint8_t> (&bytes[0], &bytes[0] + bytes.size());}
//    static std::vector<std::uint8_t>  asciiBytes(const std::string str) {
//        return std::vector<std::uint8_t> (str, str + str.length());
//    }

    
    static size_t convert_hex(uint8_t *dest, size_t count, const char *src) {
        char buf[3];
        size_t i;
        for (i = 0; i < count && *src; i++) {
            buf[0] = *src++;
            buf[1] = '\0';
            if (*src) {
                buf[1] = *src++;
                buf[2] = '\0';
            }
            if (sscanf(buf, "%hhx", &dest[i]) != 1)
                break;
        }
        return i;
    }
};
