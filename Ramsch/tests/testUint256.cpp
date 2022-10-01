#include "TestHelper.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Uint256.hpp"
#include <iostream>

struct BinaryCase
{
    const char *x;
    const char *y;
};

struct TernaryCase
{
    const char *x;
    const char *y;
    const char *z;
    unsigned int bit; // 0 or 1
};

// Global variables
static int numTestCases = 0;

static void testAdd()
{
    const vector<TernaryCase> cases{
        {"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", 0},
        {"0000000000000000000000000000000000000000000000000000000080000000", "0000000000000000000000000000000000000000000000000000000080000000", "0000000000000000000000000000000000000000000000000000000100000000", 0},
        {"000000000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFF", "0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000010000000000000000000", 0},
        {"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2E", "0000000000000000000000000000000000000000000000000000000000000001", "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 0},
        {"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000000000000000000000000", 1},
        {"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE", 1},
        {"FF00000000000000000000000000000000000000000000000000000000000000", "FF00000000000000000000000000000000000000000000000000000000000000", "FE00000000000000000000000000000000000000000000000000000000000000", 1},
    };
    for (const TernaryCase &tc : cases)
    {
        Uint256 x(tc.x);
        Uint256 y(tc.y);
        assert(x.add(y, 0) == 0);
        assert(x == Uint256(tc.x));
        assert(x.add(y, 1) == tc.bit);
        assert(x == Uint256(tc.z));
        numTestCases++;
    }
}
//
//int main()
//{
//    testAdd();
//    std::cout << "Y>OO"<< std::endl;
//    return EXIT_SUCCESS;
//}