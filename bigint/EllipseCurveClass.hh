#ifndef ELLIPSECURVECLASS_HH
#define ELLIPSECURVECLASS_HH

#include "BigIntegerLibrary.hh"
#include <math.h>
#include <stack>
#include <map>

#ifdef __linux__
	#define PASSED "\t\033[1;32m[PASSED]\033[0m"
	#define FAILED "\t\033[1;31m[FAILED]\033[0m"
#else
	#define PASSED "\t[PASSED]"
	#define FAILED "\t[FAILED]"
#endif

#define TEST_ME(function, arg1, arg2, expect) 					\
{																\
	if(function(arg1, arg2) == expect)							\
		std::cout << "res.x = " << expect.x << "\t res.y = "	\
		<< expect.y << PASSED << std::endl;						\
	else														\
		std::cout << "res.x != " << expect.x << "\t res.y != "	\
		<< expect.y << FAILED << std::endl;						\
}

struct ellipse_curve
{
    BigInteger a;
    BigInteger b;
    BigUnsigned m;
    ellipse_curve &operator=(const ellipse_curve &arg);
};

struct ellipse_curve_point
{
    BigInteger x;
    BigInteger y;

    ellipse_curve_point();
    ellipse_curve_point(BigInteger x_val, BigInteger y_val);
    ~ellipse_curve_point();
    void set_zero();
    bool operator==(const ellipse_curve_point &point);
    bool operator!=(const ellipse_curve_point &point);
};

class ellipse_curve_class
{
    ellipse_curve curve;

	bool if_point_on_curve(ellipse_curve_point &P) const;
	bool if_curve_sutable() const;
	BigInteger phi(BigInteger n) const;
    BigInteger kar_mul(BigInteger P, BigInteger n) const;
	ellipse_curve_point fill_map_with_points(ellipse_curve_point &P, std::stack<int> step,\
	std::map<int, ellipse_curve_point> &points) const;
	
    public:
    ellipse_curve_class();
    ellipse_curve_class(BigInteger a, BigInteger b, BigUnsigned m);
    ellipse_curve_class(const ellipse_curve_class &arg);
    ellipse_curve_class &operator=(const ellipse_curve_class &arg);
    ~ellipse_curve_class();
	
    ellipse_curve_point curve_point_add(ellipse_curve_point &P, ellipse_curve_point &Q) const;
    ellipse_curve_point curve_point_mul(ellipse_curve_point &P, int n) const;
    ellipse_curve get_curve() const;
};

#endif
