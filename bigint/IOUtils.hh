#ifndef IOUTILS_H
#define IOUTILS_H

#include "BigInteger.hh"
#include <string>
#include <iostream>

// std::string conversion routines.  Base 10 only.
std::string bigIntegerToString(const BigInteger &x);
BigInteger stringToBigInteger(const std::string &s);

template <class T>
BigInteger dataToBigInteger(const T* data, BigInteger::Index length, BigInteger::Sign sign);

std::ostream &operator <<(std::ostream &os, const BigUnsigned &x);
std::ostream &operator <<(std::ostream &os, const BigInteger &x);
#endif
