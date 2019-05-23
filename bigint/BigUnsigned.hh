#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H

#include "NumInBlocks.hh"
#include <string>

/* A BigUnsigned object represents a nonnegative integer of size limited only by
 * available memory. The least significant block is first,
 * and the length is such that the most significant block is nonzero. */

class BigUnsigned : protected NumInBlocks<unsigned long> {
public:
    // Enumeration for the result of a comparison.
    enum CmpRes { less = -1, equal = 0, greater = 1 };

    // BigUnsigned are built with a Block type of unsigned long.
    typedef unsigned long Block;

    typedef NumInBlocks<Block>::Index Index;
    using NumInBlocks<Block>::N;

protected:
    BigUnsigned(int, Index c) : NumInBlocks<Block>(0, c) {}

    // Decreases len to eliminate any leading zero blocks.
    void zapLeadingZeros() {
        while (len > 0 && blk[len - 1] == 0)
            len--;
    }

public:
    BigUnsigned() : NumInBlocks<Block>() {}
    BigUnsigned(const BigUnsigned &x) : NumInBlocks<Block>(x) {}
    void operator=(const BigUnsigned &x) {
        NumInBlocks<Block>::operator =(x);
    }

    // Constructor that copies from a given array of blocks.
    BigUnsigned(const Block *b, Index blen) : NumInBlocks<Block>(b, blen) {
        zapLeadingZeros();
    }

    ~BigUnsigned() {}

    // Constructors from primitive integer types
    BigUnsigned(unsigned long  x);
    BigUnsigned(         long  x);
    BigUnsigned(unsigned int   x);
    BigUnsigned(         int   x);
    BigUnsigned(unsigned short x);
    BigUnsigned(         short x);

    BigUnsigned(const std::string &s);
    BigUnsigned toDecBase() const;
    operator std::string() const;

protected:
    template <class X> void initFromPrimitive      (X x);
    template <class X> void initFromSignedPrimitive(X x);

public:

    // Converters to primitive integer types
    unsigned long  toUnsignedLong () const;
    long           toLong         () const;
    unsigned int   toUnsignedInt  () const;
    int            toInt          () const;
    unsigned short toUnsignedShort() const;
    short          toShort        () const;
protected:
    template <class X> X convertToSignedPrimitive() const;
    template <class X> X convertToPrimitive      () const;

public:

    using NumInBlocks<Block>::getCapacity;
    using NumInBlocks<Block>::getLength;

    Block getBlock(Index i) const { return i >= len ? 0 : blk[i]; }
    void setBlock(Index i, Block newBlock);
    bool isZero() const { return NumInBlocks<Block>::isEmpty(); }

    CmpRes compareTo(const BigUnsigned &x) const;

    // Ordinary comparison operators
    bool operator ==(const BigUnsigned &x) const {
        return NumInBlocks<Block>::operator ==(x);
    }
    bool operator !=(const BigUnsigned &x) const {
        return NumInBlocks<Block>::operator !=(x);
    }
    bool operator < (const BigUnsigned &x) const { return compareTo(x) == less   ; }
    bool operator <=(const BigUnsigned &x) const { return compareTo(x) != greater; }
    bool operator >=(const BigUnsigned &x) const { return compareTo(x) != less   ; }
    bool operator > (const BigUnsigned &x) const { return compareTo(x) == greater; }

    // COPY-LESS OPERATIONS

    // Arguments are read-only operands, result is saved in *this.
    void add(const BigUnsigned &a, const BigUnsigned &b);
    void subtract(const BigUnsigned &a, const BigUnsigned &b);
    void multiply(const BigUnsigned &a, const BigUnsigned &b);


    void divideWithRemainder(const BigUnsigned &b, BigUnsigned &q);

    // OVERLOADED RETURN-BY-VALUE OPERATORS
    BigUnsigned operator +(const BigUnsigned &x) const;
    BigUnsigned operator -(const BigUnsigned &x) const;
    BigUnsigned operator *(const BigUnsigned &x) const;
    BigUnsigned operator /(const BigUnsigned &x) const;
    BigUnsigned operator %(const BigUnsigned &x) const;

    // OVERLOADED ASSIGNMENT OPERATORS
    void operator +=(const BigUnsigned &x);
    void operator -=(const BigUnsigned &x);
    void operator *=(const BigUnsigned &x);
    void operator /=(const BigUnsigned &x);
    void operator %=(const BigUnsigned &x);

    // INCREMENT/DECREMENT OPERATORS
    void operator ++(   );
    void operator ++(int);
    void operator --(   );
    void operator --(int);

    friend Block getShiftedBlock(const BigUnsigned &num, Index x,
            unsigned int y);

    template <class X>
    friend X convertBigUnsignedToPrimitiveAccess(const BigUnsigned &a);
};

inline BigUnsigned BigUnsigned::operator +(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.add(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator -(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.subtract(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator *(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.multiply(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator /(const BigUnsigned &x) const {
    if (x.isZero()) throw "BigUnsigned::operator /: division by zero";
    BigUnsigned q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return q;
}
inline BigUnsigned BigUnsigned::operator %(const BigUnsigned &x) const {
    if (x.isZero()) throw "BigUnsigned::operator %: division by zero";
    BigUnsigned q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return r;
}



inline void BigUnsigned::operator +=(const BigUnsigned &x) {
    add(*this, x);
}
inline void BigUnsigned::operator -=(const BigUnsigned &x) {
    subtract(*this, x);
}
inline void BigUnsigned::operator *=(const BigUnsigned &x) {
    multiply(*this, x);
}
inline void BigUnsigned::operator /=(const BigUnsigned &x) {
    if (x.isZero()) throw "BigUnsigned::operator /=: division by zero";
    /* The following technique is slightly faster than copying *this first
     * when x is large. */
    BigUnsigned q;
    divideWithRemainder(x, q);
    // *this contains the remainder, but we overwrite it with the quotient.
    *this = q;
}
inline void BigUnsigned::operator %=(const BigUnsigned &x) {
    if (x.isZero()) throw "BigUnsigned::operator %=: division by zero";
    BigUnsigned q;
    // Mods *this by x.  Don't care about quotient left in q.
    divideWithRemainder(x, q);
}




template <class X>
void BigUnsigned::initFromPrimitive(X x) {
    if (x == 0)
        ; // NumInBlocks already initialized us to zero.
    else {
        // Create a single block.  blk is NULL; no need to delete it.
        cap = 1;
        blk = new Block[1];
        len = 1;
        blk[0] = Block(x);
    }
}

template <class X>
void BigUnsigned::initFromSignedPrimitive(X x) {
    if (x < 0)
        throw "BigUnsigned constructor: "
            "Cannot construct a BigUnsigned from a negative number";
    else
        initFromPrimitive(x);
}

template <class X>
X BigUnsigned::convertToPrimitive() const {
    if (len == 0)
        // The number is zero; return zero.
        return 0;
    else if (len == 1) {
        // The single block might fit in an X.  Try the conversion.
        X x = X(blk[0]);
        // Make sure the result accurately represents the block.
        if (Block(x) == blk[0])
            // Successful conversion.
            return x;
        // Otherwise fall through.
    }
    throw "BigUnsigned::to<Primitive>: "
        "Value is too big to fit in the requested type";
}

template <class X>
X BigUnsigned::convertToSignedPrimitive() const {
    X x = convertToPrimitive<X>();
    if (x >= 0)
        return x;
    else
        throw "BigUnsigned::to(Primitive): "
            "Value is too big to fit in the requested type";
}



#endif
