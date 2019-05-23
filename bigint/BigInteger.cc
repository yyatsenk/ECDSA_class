#include "BigInteger.hh"

void BigInteger::operator =(const BigInteger &x) {

	if (this == &x)
		return;

	sign = x.sign;
	mag = x.mag;
}

BigInteger::BigInteger(const Block *b, Index blen, Sign s) : mag(b, blen) {
	switch (s) {
	case zero:
		if (!mag.isZero())
			throw "BigInteger::BigInteger(const Block *, Index, Sign): Cannot use a sign of zero with a nonzero magnitude";
		sign = zero;
		break;
	case positive:
	case negative:
		// If the magnitude is zero, force the sign to zero.
		sign = mag.isZero() ? zero : s;
		break;
	default:
		throw "BigInteger::BigInteger(const Block *, Index, Sign): Invalid sign";
	}
}

BigInteger::BigInteger(const BigUnsigned &x, Sign s) : mag(x) {
	switch (s) {
	case zero:
		if (!mag.isZero())
			throw "BigInteger::BigInteger(const BigUnsigned &, Sign): Cannot use a sign of zero with a nonzero magnitude";
		sign = zero;
		break;
	case positive:
    case negative:
		sign = mag.isZero() ? zero : s;
		break;
	default:
		throw "BigInteger::BigInteger(const BigUnsigned &, Sign): Invalid sign";
	}
}

BigInteger::BigInteger(unsigned long  x) : mag(x) { sign = mag.isZero() ? zero : positive; }
BigInteger::BigInteger(unsigned int   x) : mag(x) { sign = mag.isZero() ? zero : positive; }
BigInteger::BigInteger(unsigned short x) : mag(x) { sign = mag.isZero() ? zero : positive; }

namespace {
	template <class X, class UX>
    BigInteger::Block magOf(X x) {
		return BigInteger::Block(x < 0 ? UX(-x) : x);
	}
	template <class X>
	BigInteger::Sign signOf(X x) {
		return (x == 0) ? BigInteger::zero
			: (x > 0) ? BigInteger::positive
			: BigInteger::negative;
	}
}

BigInteger::BigInteger(long  x) : sign(signOf(x)), mag(magOf<long , unsigned long >(x)) {}
BigInteger::BigInteger(int   x) : sign(signOf(x)), mag(magOf<int  , unsigned int  >(x)) {}
BigInteger::BigInteger(short x) : sign(signOf(x)), mag(magOf<short, unsigned short>(x)) {}

// CONVERSION TO PRIMITIVE INTEGERS

template <class X>
inline X convertBigUnsignedToPrimitiveAccess(const BigUnsigned &a) {
	return a.convertToPrimitive<X>();
}

template <class X>
X BigInteger::convertToUnsignedPrimitive() const {
	if (sign == negative)
		throw "BigInteger::to<Primitive>: "
			"Cannot convert a negative integer to an unsigned type";
	else
		return convertBigUnsignedToPrimitiveAccess<X>(mag);
}

template <class X, class UX>
X BigInteger::convertToSignedPrimitive() const {
	if (sign == zero)
		return 0;
    else if (mag.getLength() == 1) {
		Block b = mag.getBlock(0);
		if (sign == positive) {
			X x = X(b);
			if (x >= 0 && Block(x) == b)
				return x;
		} else {
            X x = -X(b);
			if (x < 0 && Block(UX(-x)) == b)
				return x;
		}
		// Otherwise fall through.
	}
	throw "BigInteger::to<Primitive>: "
		"Value is too big to fit in the requested type";
}

unsigned long  BigInteger::toUnsignedLong () const { return convertToUnsignedPrimitive<unsigned long >       (); }
unsigned int   BigInteger::toUnsignedInt  () const { return convertToUnsignedPrimitive<unsigned int  >       (); }
unsigned short BigInteger::toUnsignedShort() const { return convertToUnsignedPrimitive<unsigned short>       (); }
long           BigInteger::toLong         () const { return convertToSignedPrimitive  <long , unsigned long> (); }
int            BigInteger::toInt          () const { return convertToSignedPrimitive  <int  , unsigned int>  (); }
short          BigInteger::toShort        () const { return convertToSignedPrimitive  <short, unsigned short>(); }

// COMPARISON
BigInteger::CmpRes BigInteger::compareTo(const BigInteger &x) const {
	// A greater sign implies a greater number
	if (sign < x.sign)
		return less;
	else if (sign > x.sign)
		return greater;
	else switch (sign) {
		// If the signs are the same...
	case zero:
		return equal; // Two zeros are equal
	case positive:
		// Compare the magnitudes
		return mag.compareTo(x.mag);
	case negative:
		// Compare the magnitudes, but return the opposite result
		return CmpRes(-mag.compareTo(x.mag));
	default:
		throw "BigInteger internal error";
	}
}

#define CONDITION_OP(cond, op) \
	if (cond) { \
		BigInteger tmpThis; \
		tmpThis.op; \
		*this = tmpThis; \
		return; \
	}

void BigInteger::add(const BigInteger &a, const BigInteger &b) {
    CONDITION_OP(this == &a || this == &b, add(a, b));
	// If one argument is zero, copy the other.
	if (a.sign == zero)
		operator =(b);
	else if (b.sign == zero)
        operator =(a);
	else if (a.sign == b.sign) {
		sign = a.sign;
		mag.add(a.mag, b.mag);
    } else {
		switch (a.mag.compareTo(b.mag)) {
        case equal:
			mag = 0;
			sign = zero;
            break;
		case greater:
			sign = a.sign;
			mag.subtract(a.mag, b.mag);
			break;
		case less:
			sign = b.sign;
			mag.subtract(b.mag, a.mag);
			break;
		}
	}
}

void BigInteger::subtract(const BigInteger &a, const BigInteger &b) {
    CONDITION_OP(this == &a || this == &b, subtract(a, b));
	// If a is zero, copy b and flip its sign.  If b is zero, copy a.
	if (a.sign == zero) {
		mag = b.mag;
		sign = Sign(-b.sign);
	} else if (b.sign == zero)
        operator =(a);
	else if (a.sign != b.sign) {
		sign = a.sign;
		mag.add(a.mag, b.mag);
    } else {
        switch (a.mag.compareTo(b.mag)) {
		case equal:
			mag = 0;
			sign = zero;
            break;
		case greater:
			sign = a.sign;
			mag.subtract(a.mag, b.mag);
            break;
		case less:
			sign = Sign(-b.sign);
			mag.subtract(b.mag, a.mag);
			break;
		}
	}
}

void BigInteger::multiply(const BigInteger &a, const BigInteger &b) {
    CONDITION_OP(this == &a || this == &b, multiply(a, b));
	if (a.sign == zero || b.sign == zero) {
		sign = zero;
		mag = 0;
		return;
	}

	sign = (a.sign == b.sign) ? positive : negative;
	mag.multiply(a.mag, b.mag);
}

void BigInteger::divideWithRemainder(const BigInteger &b, BigInteger &q) {
	if (this == &q)
		throw "BigInteger::divideWithRemainder: Cannot write quotient and remainder into the same variable";
	if (this == &b || &q == &b) {
		BigInteger tmpB(b);
		divideWithRemainder(tmpB, q);
		return;
	}

	if (b.sign == zero) {
		q.mag = 0;
		q.sign = zero;
		return;
    }

	if (sign == zero) {
		q.mag = 0;
		q.sign = zero;
		return;
	}

    if (sign == b.sign) {
		q.sign = positive;
    } else {
        q.sign = negative;
		mag--;
	}

	mag.divideWithRemainder(b.mag, q.mag);

    if (sign != b.sign) {
        q.mag++;
		mag.subtract(b.mag, mag);
		mag--;
    }

	sign = b.sign;

	if (mag.isZero())
		sign = zero;
	if (q.mag.isZero())
		q.sign = zero;
}

// Negation
void BigInteger::negate(const BigInteger &a) {
    CONDITION_OP(this == &a, negate(a));

	mag = a.mag;
	sign = Sign(-a.sign);
}

// INCREMENT/DECREMENT OPERATORS

void BigInteger::operator ++() {
	if (sign == negative) {
		mag--;
		if (mag == 0)
			sign = zero;
	} else {
		mag++;
        sign = positive;
	}
}

void BigInteger::operator ++(int) {
	operator ++();
}

void BigInteger::operator --() {
	if (sign == positive) {
		mag--;
		if (mag == 0)
			sign = zero;
	} else {
		mag++;
		sign = negative;
	}
}

void BigInteger::operator --(int) {
	operator --();
}

