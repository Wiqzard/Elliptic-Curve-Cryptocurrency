#pragma once

#include "CurvePoint.hpp"
#include "Sha256Hash.hpp"
#include "Uint256.hpp"


/*
 * Performs ECDSA signature generation and verification. Provides just three static functions.
 */
class Ecdsa final {
    
    // Computes the signature (deterministically) when given the private key, message hash, and random nonce.
    public:
    static bool sign(const Uint256 &privateKey, const Sha256Hash &msgHash, const Uint256 &nonce, Uint256 &outR, Uint256 &outS);
    
    
    // Computes a deterministic nonce based on the HMAC-SHA-256 of the message hash with the private key,and then performs ECDSA signing. .
    static bool signWithHmacNonce(const Uint256 &privateKey, const Sha256Hash &msgHash, Uint256 &outR, Uint256 &outS);
    
    
    // Checks whether the given signature, message, and public key are valid together. The public key point must be normalized.
    static bool verify(const CurvePoint &publicKey, const Sha256Hash &msgHash, const Uint256 &r, const Uint256 &s);
    
    
    // Computes x = (x * y) % CurvePoint::ORDER. Requires x < CurvePoint::ORDER, but y is unrestricted.
    private: static void multiplyModOrder(Uint256 &x, const Uint256 &y);
    
    
    Ecdsa() = delete;  // Not instantiable
    
};
