#include <string>
#include <iostream>

#include "BigIntegerLibrary.hh"
#include "EllipseCurveClass.hh"


int main() {
	try {
        ellipse_curve_class curve(BigInteger(-1), BigInteger(1), BigUnsigned(std::string("751")));
        ellipse_curve_class curve_1(BigInteger(1), BigInteger(1), BigUnsigned(std::string("23")));
		
		ellipse_curve_point P(BigInteger(78), BigInteger(480));
		ellipse_curve_point Q(BigInteger(16), BigInteger(102));

		ellipse_curve_point P_1(BigInteger(1), BigInteger(16));
		ellipse_curve_point Q_1(BigInteger(19), BigInteger(18));
		
		ellipse_curve_point res;
		

		/* Example of usage :

			res = curve.curve_point_add(P, Q); 		where P and Q are ellipse curve points which are located on curve
			res = curve.curve_point_mul(P, n); 		where n is positive int

			For more info look at function prototypes in EllipseCurveClass.hpp

		*/

		// TEST_ME(function, arg1, arg2, expect)

		TEST_ME(curve.curve_point_mul, P, 147, ellipse_curve_point(463, 15));
		TEST_ME(curve.curve_point_mul, P, 0, ellipse_curve_point(0, 0));
		TEST_ME(curve.curve_point_mul, P, -1, ellipse_curve_point(0, 0));
		TEST_ME(curve_1.curve_point_mul, P_1, 7, ellipse_curve_point(11, 20));
		TEST_ME(curve_1.curve_point_mul, P_1, 1, ellipse_curve_point(1, 16));
		TEST_ME(curve_1.curve_point_mul, P_1, 0, ellipse_curve_point(0, 0));
		TEST_ME(curve_1.curve_point_add, P_1, Q_1, ellipse_curve_point(5, 4));
		TEST_ME(curve_1.curve_point_add, P, Q, ellipse_curve_point(16, 13));
	}
	catch(...)
	{
		std::cout << "Error happened\n";
	}

	return 0;
}
