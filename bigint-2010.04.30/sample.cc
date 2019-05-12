// Sample program demonstrating the use of the Big Integer Library.

// Standard libraries
#include <string>
#include <iostream>

// `BigIntegerLibrary.hh' includes all of the library headers.
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

};


int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		ellipse_curve_class curve(BigInteger(2), BigInteger(3), BigUnsigned(stringToBigUnsigned(std::string("3141592"))));
		ellipse_curve_point P(BigInteger(stringToBigInteger(std::string("314159265358973279"))), BigInteger(stringToBigInteger(std::string("3145358979113238462643383279"))));
		ellipse_curve_point Q(BigInteger(95), BigInteger(31));
		ellipse_curve_point res;
		
		res = curve.curve_point_add(P, Q);
		std::cout<< res.x << "\n" << res.y << std::endl;
		BigUnsigned m(13);
		BigInteger dx;
		BigInteger dy;
		BigInteger l;
		BigInteger res_x;
		BigInteger res_y;
		BigInteger test(123422232);
		
		/*dx = (q_x - p_x);
		dy = (q_y - p_y);
		//if (dx > dy)
		//	l = ((q_y - p_y) * p_x.toPow((q_x - p_x), (phi(m) - 1)))%m;
		//else
		//	l = (q_y - p_y)/(q_x - p_x)%m;
		//res_x = (l.toPow(l, 2) - p_x - q_x)%m;
		res_y = (l*(p_x - res_x)-p_y)%m;
		//std::cout << res_x << "\n" << res_y << std::endl;//phi((q_y - p_y));
		//std::cout << (test * test *test *test*test*test)/(test * test *test *test*test*test);
		*/
	}
	catch(...)
	{}
		/* Any primitive integer can be converted implicitly to a
		 * BigInteger. */
		//a = b;

		/* The reverse conversion requires a method call (implicit
		 * conversions were previously supported but caused trouble).
		 * If a were too big for an int, the library would throw an
		 * exception. */
		//b = a.toInt();

		//BigInteger c(a); // Copy a BigInteger.

		// The int literal is converted to a BigInteger.
		//BigInteger d(-314159265);

		/* This won't compile (at least on 32-bit machines) because the
		 * number is too big to be a primitive integer literal, and
		 * there's no such thing as a BigInteger literal. */
		//BigInteger e(3141592653589793238462643383279);

		// Instead you can convert the number from a string.
		//std::string s("3141592653589793238462643383279");
		//BigInteger f = stringToBigInteger(s);

		// You can convert the other way too.
		//std::string s2 = bigIntegerToString(f); 

		// f is implicitly stringified and sent to std::cout.
		//std::cout << f << std::endl;

		/* Let's do some math!  The library overloads most of the
		 * mathematical operators (including assignment operators) to
		 * work on BigIntegers.  There are also ``copy-less''
		 * operations; see `BigUnsigned.hh' for details. */

		// Arithmetic operators
		//BigInteger g(314159), h(265);
		//std::cout << (g + h) << '\n'
		//	<< (g - h) << '\n'
		//	<< (g * h) << '\n'
		//	<< (g / h) << '\n'
		//	<< (g % h) << std::endl;

		// Bitwise operators
		

	return 0;
}

/*
The original sample program produces this output:

3141592653589793238462643383279
314424
313894
83252135
1185
134
0xFF
0xFF00FFFF
0xFF00FF00
0x1FFFE00000
0x3F
314^0 = 1
314^1 = 314
314^2 = 98596
314^3 = 30959144
314^4 = 9721171216
314^5 = 3052447761824
314^6 = 958468597212736
314^7 = 300959139524799104
314^8 = 94501169810786918656
314^9 = 29673367320587092457984
314^10 = 9317437338664347031806976
12
8
1931

*/
