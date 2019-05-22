#include "BigIntegerUtils.hh"

std::string bigIntegerToString(const BigInteger &x) {
    return (x.getSign() == BigInteger::negative)
        ? (std::string("-") + std::string(x.getMagnitude()))
        : (std::string(x.getMagnitude()));
}

BigInteger stringToBigInteger(const std::string &s) {
    // Recognize a sign followed by a BigUnsigned.
    return (s[0] == '-') ? BigInteger(BigUnsigned(s.substr(1, s.length() - 1)), BigInteger::negative)
        : (s[0] == '+') ? BigInteger(BigUnsigned(s.substr(1, s.length() - 1)))
        : BigInteger(BigUnsigned(s));
}

std::ostream &operator <<(std::ostream &os, const BigUnsigned &x) {
//    long osFlags = os.flags();

    std::string s1 = std::string(x);
    os << s1;
    return os;
}

std::ostream &operator <<(std::ostream &os, const BigInteger &x) {
    if (x.getSign() == BigInteger::negative)
        os << '-';
    os << x.getMagnitude();
    return os;
}
