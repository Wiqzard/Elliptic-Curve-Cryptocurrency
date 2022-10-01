#pragma once

#include <cstddef>
#include <cstdint>
#include "Sha256Hash.hpp"


/*
 * Computes the SHA-256 hash of a sequence of bytes, returning a Sha256Hash object.
 * Provides three static methods, and an instantiable stateful hasher.
 */

class Sha256 final {
    
    /*---- Public constants ----*/
    
    public:static constexpr int BLOCK_LEN = 64;  // In bytes
    
    
    
    /*---- Instance members ----*/
    // https://en.wikipedia.org/wiki/SHA-2
    //Initialize hash values
    private: std::uint32_t state[8] = {
        UINT32_C(0x6A09E667), UINT32_C(0xBB67AE85), UINT32_C(0x3C6EF372), UINT32_C(0xA54FF53A),
        UINT32_C(0x510E527F), UINT32_C(0x9B05688C), UINT32_C(0x1F83D9AB), UINT32_C(0x5BE0CD19),
    };
    private: std::uint64_t length;
    private: std::uint8_t buffer[BLOCK_LEN];
    private: int bufferLen;
    
    
    // Constructs a new SHA-256 hasher with an initially blank message.
    public:
    explicit Sha256();
    
    
    // Appends message bytes to this ongoing hasher, and returns this object itself.
    Sha256 &append(const std::uint8_t bytes[], std::size_t len);
    
    
    // Returns the SHA-256 hash of all the bytes seen. Destroys the state so that no further append() or getHash() will be valid.
    Sha256Hash getHash();
    
    
    
    /*---- Static functions ----*/
    
    static Sha256Hash getHash(const std::uint8_t msg[], std::size_t len);
    
    
    static Sha256Hash getDoubleHash(const std::uint8_t msg[], std::size_t len);
    
    
    static Sha256Hash getHmac(const std::uint8_t key[], std::size_t keyLen, const std::uint8_t msg[], std::size_t msgLen);
    
    
    static void compress(std::uint32_t state[8], const std::uint8_t block[BLOCK_LEN]);
    
    
    // Requires 1 <= i <= 31
    private: static std::uint32_t rotr32(std::uint32_t x, int i);
    
    
    
    /*---- Private constants ----*/
    
    private: static constexpr int NUM_ROUNDS = 64;
    private: static const std::uint32_t ROUND_CONSTANTS[NUM_ROUNDS];
    
};
