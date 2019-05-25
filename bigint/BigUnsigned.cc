#include "BigUnsigned.hh"

BigUnsigned::BigUnsigned(unsigned long  x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(unsigned int   x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(unsigned short x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(         long  x) { initFromSignedPrimitive(x); }
BigUnsigned::BigUnsigned(         int   x) { initFromSignedPrimitive(x); }
BigUnsigned::BigUnsigned(         short x) { initFromSignedPrimitive(x); }


BigUnsigned::BigUnsigned(const std::string &s)
{
    len = Index(s.length());
    allocate(len);

    Index digitNum, symbolNumInString;
    for (digitNum = 0; digitNum < len; digitNum++) {
        symbolNumInString = len - 1 - digitNum;
        char theSymbol = s[symbolNumInString];
        if (theSymbol >= '0' && theSymbol <= '9')
            blk[digitNum] = theSymbol - '0';
        else
            throw "BigUnsignedInABase(std::string, Base): Bad symbol in input.  Only 0-9, A-Z, a-z are accepted.";
    }
    zapLeadingZeros();

    BigUnsigned ans(0), buBase(10), temp;
    digitNum = len;
    while (digitNum > 0) {
        digitNum--;
        temp.multiply(ans, buBase);
        ans.add(temp, BigUnsigned(blk[digitNum]));
    }

    *this = ans;
}

unsigned long  BigUnsigned::toUnsignedLong () const { return convertToPrimitive      <unsigned long >(); }
unsigned int   BigUnsigned::toUnsignedInt  () const { return convertToPrimitive      <unsigned int  >(); }
unsigned short BigUnsigned::toUnsignedShort() const { return convertToPrimitive      <unsigned short>(); }
long           BigUnsigned::toLong         () const { return convertToSignedPrimitive<         long >(); }
int            BigUnsigned::toInt          () const { return convertToSignedPrimitive<         int  >(); }
short          BigUnsigned::toShort        () const { return convertToSignedPrimitive<         short>(); }

void BigUnsigned::setBlock(Index i, Block newBlock) {
    if (newBlock == 0) {
        if (i < len) {
            blk[i] = 0;
            zapLeadingZeros();
        }
        // If i >= len, no effect.
    } else {
        if (i >= len) {
            allocateAndCopy(i+1);
            for (Index j = len; j < i; j++)
                blk[j] = 0;
            len = i+1;
        }
        blk[i] = newBlock;
    }
}

// COMPARISON
BigUnsigned::CmpRes BigUnsigned::compareTo(const BigUnsigned &x) const {
    if (len < x.len)
        return less;
    else if (len > x.len)
        return greater;
    else {
        // Compare blocks one by one from left to right.
        Index i = len;
        while (i > 0) {
            i--;
            if (blk[i] == x.blk[i])
                continue;
            else if (blk[i] > x.blk[i])
                return greater;
            else
                return less;
        }
        // If no blocks differed, the numbers are equal.
        return equal;
    }
}


#define CONDITION_OP(cond, op) \
    if (cond) { \
        BigUnsigned tmpThis; \
        tmpThis.op; \
        *this = tmpThis; \
        return; \
    }


void BigUnsigned::add(const BigUnsigned &a, const BigUnsigned &b) {
    CONDITION_OP(this == &a || this == &b, add(a, b));
    // If one argument is zero, copy the other.
    if (a.len == 0) {
        operator =(b);
        return;
    } else if (b.len == 0) {
        operator =(a);
        return;
    }
    // Some variables...
    // Carries in and out of an addition stage
    bool carryIn, carryOut;
    Block temp;
    Index i;
    // a2 points to the longer input, b2 points to the shorter
    const BigUnsigned *a2, *b2;
    if (a.len >= b.len) {
        a2 = &a;
        b2 = &b;
    } else {
        a2 = &b;
        b2 = &a;
    }
    // Set prelimiary length and make room in this BigUnsigned
    len = a2->len + 1;
    allocate(len);
    // For each block index that is present in both inputs...
    for (i = 0, carryIn = false; i < b2->len; i++) {
        // Add input blocks
        temp = a2->blk[i] + b2->blk[i];
        carryOut = (temp < a2->blk[i]);
        // If a carry was input, handle it
        if (carryIn) {
            temp++;
            carryOut |= (temp == 0);
        }
        blk[i] = temp; // Save the addition result
        carryIn = carryOut; // Pass the carry along
    }
    for (; i < a2->len && carryIn; i++) {
        temp = a2->blk[i] + 1;
        carryIn = (temp == 0);
        blk[i] = temp;
    }
    for (; i < a2->len; i++)
        blk[i] = a2->blk[i];
    // Set the extra block if there's still a carry, decrease length otherwise
    if (carryIn)
        blk[i] = 1;
    else
        len--;
}

void BigUnsigned::subtract(const BigUnsigned &a, const BigUnsigned &b) {
    CONDITION_OP(this == &a || this == &b, subtract(a, b));
    if (b.len == 0) {
        // If b is zero, copy a.
        operator =(a);
        return;
    } else if (a.len < b.len)
        // If a is shorter than b, the result is negative.
        throw "BigUnsigned::subtract: "
            "Negative result in unsigned calculation";
    // Some variables...
    bool borrowIn, borrowOut;
    Block temp;
    Index i;
    // Set preliminary length and make room
    len = a.len;
    allocate(len);
    // For each block index that is present in both inputs...
    for (i = 0, borrowIn = false; i < b.len; i++) {
        temp = a.blk[i] - b.blk[i];
        // If a reverse rollover occurred,
        // the result is greater than the block from a.
        borrowOut = (temp > a.blk[i]);
        // Handle an incoming borrow
        if (borrowIn) {
            borrowOut |= (temp == 0);
            temp--;
        }
        blk[i] = temp; // Save the subtraction result
        borrowIn = borrowOut; // Pass the borrow along
    }
    for (; i < a.len && borrowIn; i++) {
        borrowIn = (a.blk[i] == 0);
        blk[i] = a.blk[i] - 1;
    }
    if (borrowIn) {
        len = 0;
        throw "BigUnsigned::subtract: Negative result in unsigned calculation";
    } else
        // Copy over the rest of the blocks
        for (; i < a.len; i++)
            blk[i] = a.blk[i];
    // Zap leading zeros
    zapLeadingZeros();
}

inline BigUnsigned::Block getShiftedBlock(const BigUnsigned &num,
    BigUnsigned::Index x, unsigned int y) {
    BigUnsigned::Block part1 = (x == 0 || y == 0) ? 0 : (num.blk[x - 1] >> (BigUnsigned::N - y));
    BigUnsigned::Block part2 = (x == num.len) ? 0 : (num.blk[x] << y);
    return part1 | part2;
}

void BigUnsigned::multiply(const BigUnsigned &a, const BigUnsigned &b) {
    CONDITION_OP(this == &a || this == &b, multiply(a, b));
    // If either a or b is zero, set to zero.
    if (a.len == 0 || b.len == 0) {
        len = 0;
        return;
    }

    Index i, j, k;
    unsigned int i2;
    Block temp;
    bool carryIn, carryOut;
    // Set preliminary length and make room
    len = a.len + b.len;
    allocate(len);
    // Zero out this object
    for (i = 0; i < len; i++)
        blk[i] = 0;
    // For each block of the first number...
    for (i = 0; i < a.len; i++) {
        // For each 1-bit of that block...
        for (i2 = 0; i2 < N; i2++) {
            if ((a.blk[i] & (Block(1) << i2)) == 0)
                continue;

            for (j = 0, k = i, carryIn = false; j <= b.len; j++, k++) {
                temp = blk[k] + getShiftedBlock(b, j, i2);
                carryOut = (temp < blk[k]);
                if (carryIn) {
                    temp++;
                    carryOut |= (temp == 0);
                }
                blk[k] = temp;
                carryIn = carryOut;
            }
            for (; carryIn; k++) {
                blk[k]++;
                carryIn = (blk[k] == 0);
            }
        }
    }
    // Zap possible leading zero
    if (blk[len - 1] == 0)
        len--;
}

namespace {
    unsigned int bitLen(unsigned int x) {
        unsigned int len = 0;
        while (x > 0) {
            x >>= 1;
            len++;
        }
        return len;
    }
    unsigned int ceilingDiv(unsigned int a, unsigned int b) {
        return (a + b - 1) / b;
    }
}

BigUnsigned BigUnsigned::toDecBase() const
{
    int base = 10;
    int maxBitLenOfX = len * BigUnsigned::N;
    int minBitsPerDigit = bitLen(base) - 1;
    int maxDigitLenOfX = ceilingDiv(maxBitLenOfX, minBitsPerDigit);
    Index newlen = maxDigitLenOfX;

    BigUnsigned x, copy, buBase(base);
    x.allocate(newlen);
    Index digitNums = 0;
    copy = * this;

    while (!copy.isZero()) {
        BigUnsigned lastDigit(copy);
        lastDigit.divideWithRemainder(buBase, copy);
        x.setBlock(digitNums, lastDigit.toUnsignedShort());
        digitNums++;
    }

    return x;
}


void BigUnsigned::divideWithRemainder(const BigUnsigned &b, BigUnsigned &q) {
    if (this == &q)
        throw "BigUnsigned::divideWithRemainder: Cannot write quotient and remainder into the same variable";

    if (this == &b || &q == &b) {
        BigUnsigned tmpB(b);
        divideWithRemainder(tmpB, q);
        return;
    }

    if (b.len == 0) {
        q.len = 0;
        return;
    }

    if (len < b.len) {
        q.len = 0;
        return;
    }

    Index i, j, k;
    unsigned int i2;
    Block temp;
    bool borrowIn, borrowOut;

    Index origLen = len; // Save real length.
    allocateAndCopy(len + 1);
    len++;
    blk[origLen] = 0; // Zero the added block.

    Block *subtractBuf = new Block[len];
    q.len = origLen - b.len + 1;
    q.allocate(q.len);
    // Zero out the quotient
    for (i = 0; i < q.len; i++)
        q.blk[i] = 0;

    // For each possible left-shift of b in blocks...
    i = q.len;
    while (i > 0) {
        i--;
        // For each possible left-shift of b in bits...
        q.blk[i] = 0;
        i2 = N;
        while (i2 > 0) {
            i2--;

            for (j = 0, k = i, borrowIn = false; j <= b.len; j++, k++) {
                temp = blk[k] - getShiftedBlock(b, j, i2);
                borrowOut = (temp > blk[k]);
                if (borrowIn) {
                    borrowOut |= (temp == 0);
                    temp--;
                }
                subtractBuf[k] = temp;
                borrowIn = borrowOut;
            }
            for (; k < origLen && borrowIn; k++) {
                borrowIn = (blk[k] == 0);
                subtractBuf[k] = blk[k] - 1;
            }

            if (!borrowIn) {
                q.blk[i] |= (Block(1) << i2);
                while (k > i) {
                    k--;
                    blk[k] = subtractBuf[k];
                }
            }
        }
    }
    if (q.blk[q.len - 1] == 0)
        q.len--;

    zapLeadingZeros();
    delete [] subtractBuf;
}


// INCREMENT/DECREMENT OPERATORS

void BigUnsigned::operator ++() {
    Index i;
    bool carry = true;
    for (i = 0; i < len && carry; i++) {
        blk[i]++;
        carry = (blk[i] == 0);
    }
    if (carry) {
        allocateAndCopy(len + 1);
        len++;
        blk[i] = 1;
    }
}

void BigUnsigned::operator ++(int) {
    operator ++();
}

// Prefix
void BigUnsigned::operator --() {
    if (len == 0)
        throw "BigUnsigned::operator --(): Cannot decrement an unsigned zero";
    Index i;
    bool borrow = true;
    for (i = 0; borrow; i++) {
        borrow = (blk[i] == 0);
        blk[i]--;
    }
    if (blk[len - 1] == 0)
        len--;
}

// Postfix
void BigUnsigned::operator --(int) {
    operator --();
}

BigUnsigned::operator std::string() const {
    if (len == 0)
        return std::string("0");

    BigUnsigned decimal = toDecBase();

    char *s = new char[decimal.getLength() + 1];
    s[decimal.getLength()] = '\0';
    Index digitNum, symbolNumInString;
    for (symbolNumInString = 0; symbolNumInString < decimal.getLength(); symbolNumInString++) {
        digitNum = decimal.getLength() - 1 - symbolNumInString;
        unsigned short theDigit = decimal.getBlock(digitNum);
        if (theDigit < 10)
            s[symbolNumInString] = char('0' + theDigit);
        else
            s[symbolNumInString] = char('A' + theDigit - 10);
    }
    std::string s2(s);
    delete [] s;

    return s2;
}
