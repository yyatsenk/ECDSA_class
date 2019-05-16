#ifndef ELLIPSECURVECLASS_HH
#define ELLIPSECURVECLASS_HH

#include "BigIntegerLibrary.hh"
#include <math.h>
#include <stack>
#include <map>
#include <assert.h>


#define TEST_ME(function, arg1, arg2, expect) 										\
{																					\
	assert(function(arg1, arg2) == expect);											\
	std::cout << "res.x = " << expect.x << "\t res.y = " << expect.y << std::endl;	\
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
	public:
	ellipse_curve_class();
	ellipse_curve_class(BigInteger a, BigInteger b, BigUnsigned m);
	ellipse_curve_class(const ellipse_curve_class &arg);
	ellipse_curve_class &operator=(const ellipse_curve_class &arg);
	virtual ~ellipse_curve_class();
	
	BigInteger phi(BigInteger n) const;
	ellipse_curve_point recurs(ellipse_curve_point &P, int n, std::stack<int> step, std::map<int, ellipse_curve_point> &points);
	ellipse_curve_point curve_point_add(ellipse_curve_point &P, ellipse_curve_point &Q) const;
	ellipse_curve_point curve_point_mul(ellipse_curve_point &P, int n);
	ellipse_curve_point curve_point_mul(ellipse_curve_point &P, BigInteger n) const;
	bool if_point_on_curve(ellipse_curve_point &P) const;
	bool if_curve_sutable() const;
    BigInteger kar_mul(BigInteger P, BigInteger n) const;
	ellipse_curve get_curve() const;
};

#endif