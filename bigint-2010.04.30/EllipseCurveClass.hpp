#ifndef ELLIPSECURVECLASS_HPP
#define ELLIPSECURVECLASS_HPP

#include "BigIntegerLibrary.hh"

struct ellipse_curve
{
	BigInteger a;
	BigInteger b;
	BigUnsigned m;
};
struct ellipse_curve_point
{
	BigInteger x;
	BigInteger y;
	bool is_infinitive;
	ellipse_curve_point()
	{}
	ellipse_curve_point(BigInteger &&x_val, BigInteger &&y_val):x(x_val),y(y_val)
	{}
	bool operator==(const ellipse_curve_point &point)
	{
		if (x == point.x && y == point.y)
			return 1;
		return 0;
	}
};

class ellipse_curve_class
{
	ellipse_curve curve;
	public:
	ellipse_curve_class()
	{}
	ellipse_curve_class(BigInteger &&a, BigInteger &&b, BigUnsigned &&m)
	{
		curve.a = a;
		curve.b = b;
		curve.m = m;
	}
	ellipse_curve_class(const ellipse_curve_class &arg)
	{
		curve.a = arg.curve.a;
		curve.b = arg.curve.b;
		curve.m = arg.curve.m;
	}
	~ellipse_curve_class(){}
	BigInteger phi(BigInteger n) {
	BigInteger ret(1);
	for(BigInteger i(2); i * i <= n; i++) {
		BigInteger p(1);
		while(n % i == BigInteger(0)) {
			p = p * i;
			n = n / i;
		}
		p = p / i;
		if(!p.isZero() && p >= BigInteger(1))
		{
			ret = ret * p * (i - BigInteger(1));
			p = p / i;
		}
	}
	n = n - 1;
	if (n != BigInteger(0))
		return n * ret;
	return ret;
}
	ellipse_curve_point curve_point_add(ellipse_curve_point &P, ellipse_curve_point &Q)
	{
		ellipse_curve_point res;
		BigInteger lambda;
		
		if ((Q.x - P.x) > (Q.y - P.y))
			lambda = ((Q.y - P.y) * (Q.x - P.x).toPow(phi(curve.m) - 1)) % curve.m;
		else if (P == Q)
		{
			if ((BigInteger(3) * P.x.toPow(2) + curve.a) < BigInteger(2) * P.y)
				lambda = ((BigInteger(3) * P.x.toPow(2) + curve.a) * (BigInteger(2) * P.y).toPow(phi(curve.m) - 1))%curve.m;
			else
				lambda = ((BigInteger(3) * P.x.toPow(2) + curve.a) / (BigInteger(2) * P.y))%curve.m;
		}
		else
			lambda = (Q.y - P.y) / (Q.x - P.x) % curve.m;
		res.x = (lambda.toPow(2) -P.x -Q.x)%curve.m;
		res.y = (lambda * (P.x - res.x) - P.y) %curve.m;
		
		return (res);
	}
/*
    n = max(размер X, размер Y)
    если n = 1: вернуть X * Y
    X_l = левые n/2 цифр X
    X_r = правые n/2 цифр X
    Y_l = левые n/2 цифр Y
    Y_r = правые n/2 цифр Y
    Prod1 = Karatsuba_mul(X_l, Y_l)
    Prod2 = Karatsuba_mul(X_r, Y_r)
    Prod3 = Karatsuba_mul(X_l + X_r, Y_l + Y_r)
    вернуть Prod1 * 10 ^ n + (Prod3 - Prod1 - Prod2) * 10 ^ (n / 2) + Prod2*/
    ellipse_curve_point curve_point_mul(ellipse_curve_point &P, BigInteger &n)
	{
        ellipse_curve_point tmp;
        unsigned int len = n.getLength() +1;
        std::cout <<"Test " << len << std::endl;
        BigInteger X_l = P.x / BigInteger((10 *(len - 1)));
        BigInteger X_r = P.x % BigInteger((10 *(len - 1)));
        BigInteger Y_l = n / BigInteger((10 *(len - 1)));
        BigInteger Y_r = n % BigInteger((10 *(len - 1)));
        BigInteger r1 = X_l * Y_l;
        BigInteger r2 = X_r * Y_r;
        BigInteger r3 = (X_l + X_r) * (Y_l + Y_r);
        BigInteger res;
        std::cout <<"r1 = " << r1 << " r2 = "  << r2 << " r3 = " << r3 << " Y_r = " << Y_r<< std::endl;
        res = r1 * BigInteger(10).toPow(len) + (r3 - r1 - r2) * BigInteger(10).toPow(len / 2) + r2;
        std::cout <<"res = " << res << std::endl;
        return tmp;
    }

};

#endif