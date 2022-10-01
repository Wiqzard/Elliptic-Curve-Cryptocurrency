#include <cassert>
#include <cstdint>
#include <cstring>

#include "Ecdsa.hpp"
#include "FieldInt.hpp"
#include "Sha256.hpp"

using std::uint8_t;
using std::uint32_t;


bool Ecdsa::sign(const Uint256 &privateKey, const Sha256Hash &msgHash, const Uint256 &nonce, Uint256 &outR, Uint256 &outS) {
    /*
      Algorithm: https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm
      Steps 1. and 2. can be ignored sice we work with fixed size message hashes
      Step 3. also already done. (select integer k in range [1, order-1]))
      4. Check if nonce in range range [1, order-1]
      4. Calculate curve point (x_1, y_1) = k*G   -> done in next step
      5. r = x_1 mod order -> if r = 0 go back to 3. -> r = p.x % order
      6. s = k^-1(msgHash + r*privateKey) mod order -> if s=0 go back to 3.
      7. r, s are the signature
     */
    
    const Uint256 &order = CurvePoint::ORDER;
    const Uint256 &zero = Uint256::ZERO;
    if (nonce == zero || nonce >= order)
        return false;
    
    const CurvePoint p = CurvePoint::privateExponentToPublicPoint(nonce);
    Uint256 r(p.x);
    r.subtract(order, static_cast<uint32_t>(r >= order));
    if (r == zero)
        return false;
    assert(r < order);
    
    Uint256 s = r;
    const Uint256 z(msgHash.value);
    multiplyModOrder(s, privateKey);
    uint32_t carry = s.add(z);
    s.subtract(order, carry | static_cast<uint32_t>(s >= order));

    Uint256 kInv = nonce;
    kInv.reciprocal(order);
    multiplyModOrder(s, kInv);
    if (s == zero)
        return false;
    
    Uint256 negS = order;
    negS.subtract(s);
    s.replace(negS, static_cast<uint32_t>(negS < s));  // To ensure low S values for BIP 62
    outR = r;
    outS = s;
    return true;
}


bool Ecdsa::signWithHmacNonce(const Uint256 &privateKey, const Sha256Hash &msgHash, Uint256 &outR, Uint256 &outS) {
    uint8_t privkeyBytes[Uint256::NUM_WORDS * 4];
    privateKey.getBigEndianBytes(privkeyBytes);
    const Sha256Hash hmac = Sha256::getHmac(privkeyBytes, sizeof(privkeyBytes), msgHash.value, Sha256Hash::HASH_LEN);
    const Uint256 nonce(hmac.value);
    return sign(privateKey, msgHash, nonce, outR, outS);
}


bool Ecdsa::verify(const CurvePoint &publicKey, const Sha256Hash &msgHash, const Uint256 &r, const Uint256 &s) {
    /*
     * Algorithm pseudocode:
     * if (pubKey == zero || !(pubKey is normalized) ||
     *     !(pubKey on curve) || n * pubKey != zero)
     *   return false
     * if (!(0 < r, s < order))
     *   return false
     * w = s^-1 % order
     * u1 = (msgHash * w) % order
     * u2 = (r * w) % order
     * p = u1 * G + u2 * pubKey
     * if (p == zero)
     *   return false
     * return r == p.x % order
     */
    
    const Uint256 &order = CurvePoint::ORDER;
    const Uint256 &zero = Uint256::ZERO;
    if (!(zero < r && r < order && zero < s && s < order))
        return false;
    
    CurvePoint q = publicKey;
    q.multiply(CurvePoint::ORDER);
    if (publicKey.isZero() || publicKey.z != CurvePoint::FI_ONE || !publicKey.isOnCurve() || !q.isZero())
        return false;
    
    Uint256 w = s;
    w.reciprocal(order);
    const Uint256 z(msgHash.value);
    Uint256 u1 = w;
    Uint256 u2 = w;
    multiplyModOrder(u1, z);
    multiplyModOrder(u2, r);
    
    CurvePoint p = CurvePoint::G;
    q = publicKey;
    p.multiply(u1);
    q.multiply(u2);
    p.add(q);
    p.normalize();
    if (p.isZero())
        return false;
    
    Uint256 px(p.x);
    px.subtract(order, static_cast<uint32_t>(px >= order));
    return r == px;
}


void Ecdsa::multiplyModOrder(Uint256 &x, const Uint256 &y) {
    /*
     * Russian peasant multiplication with modular reduction at each step. Algorithm pseudocode:
     * z = 0
     * for (i = 255 .. 0) {
     *   z = (z * 2) % order
     *   if (y.bit[i] == 1)
     *     z = (z + x) % order
     * }
     * x = z
     */
    const Uint256 &mod = CurvePoint::ORDER;
    assert(&x != &y && x < mod);
    Uint256 z = Uint256::ZERO;
    
    for (int i = Uint256::NUM_WORDS * 32 - 1; i >= 0; i--) {
        // Multiply by 2
        uint32_t c = z.shiftLeft1();
        z.subtract(mod, c | static_cast<uint32_t>(z >= mod));
        // Conditionally add x
        uint32_t enable = (y.value[i >> 5] >> (i & 31)) & 1;
        c = z.add(x, enable);
        z.subtract(mod, c | static_cast<uint32_t>(z >= mod));
        assert(z < mod);
    }
    x = z;
}
