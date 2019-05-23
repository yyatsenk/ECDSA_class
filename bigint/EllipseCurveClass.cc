#include "EllipseCurveClass.hh"

ellipse_curve &ellipse_curve::operator=(const ellipse_curve &arg)
{
    if (this == &arg)
        return (*this);
    a = arg.a;
    b = arg.b;
    m = arg.m;
    
    return (*this);
}

ellipse_curve_point::ellipse_curve_point()
{}
ellipse_curve_point::~ellipse_curve_point()
{}
ellipse_curve_point::ellipse_curve_point(BigInteger x_val, BigInteger y_val):x(x_val),y(y_val)
{}

void ellipse_curve_point::set_zero()
{
    x = 0;
    y = 0;
}

bool ellipse_curve_point::operator==(const ellipse_curve_point &point)
{
	if (x == point.x && y == point.y)
		return 1;
	return 0;
}

bool ellipse_curve_point::operator!=(const ellipse_curve_point &point)
{
	return point.x != x || point.y != y;
}

ellipse_curve_class::ellipse_curve_class()
{}
ellipse_curve_class::ellipse_curve_class(BigInteger a, BigInteger b, BigUnsigned m)
{
	curve.a = a;
	curve.b = b;
	curve.m = m;
}
ellipse_curve_class::ellipse_curve_class(const ellipse_curve_class &arg)
{
	curve = arg.curve;
}
ellipse_curve_class::~ellipse_curve_class()
{}

ellipse_curve_class &ellipse_curve_class::operator=(const ellipse_curve_class &arg)
{
    if (this == &arg)
        return (*this);
    curve = arg.curve;
    return (*this);
    
}
BigInteger ellipse_curve_class::phi(BigInteger n) const
{
	BigInteger ret(1);

	for(BigInteger i(2); i * i <= n; i++)
	{
		BigInteger p(1);
		while(n % i == BigInteger(0))
		{
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

ellipse_curve_point ellipse_curve_class::curve_point_add(ellipse_curve_point &P, ellipse_curve_point &Q) const
{
	ellipse_curve_point res;
	BigInteger lambda;

	if (P != Q)
		lambda = (kar_mul((Q.y - P.y) ,(Q.x - P.x).toPow(phi(curve.m) - 1))) % curve.m;
	else
		lambda = (kar_mul((kar_mul(BigInteger(3) , kar_mul(P.x, P.x)) + curve.a), (BigInteger(2) * P.y).toPow(phi(curve.m) - 1)))%curve.m;
	res.x = (kar_mul(lambda, lambda) -P.x - Q.x) % curve.m;
	res.y = (kar_mul(lambda ,(P.x - res.x)) - P.y) % curve.m;

	return (res);
}

ellipse_curve_point ellipse_curve_class::fill_map_with_points(ellipse_curve_point &P, std::stack<int> step,\
std::map<int, ellipse_curve_point> &points) const
{
	ellipse_curve_point res;
	int len = step.size();
	std::stack<int> stack_copy(step);

	if (len == 1 && step.top() == 0)
	{
		points.insert(std::make_pair(0, P));
		return P;
	}
	else if (len == 1 && step.top() != 0)
	{
		int i = -1;
		res = P;
		while (++i < step.top())
			res = curve_point_add(res, res);
		points.insert(std::make_pair(i, res));
		return res;
	}
	if (points.find(step.top() - 1) == points.end())
	{
		stack_copy.pop();
		fill_map_with_points(P, stack_copy, points);
	}
	if (step.top() != 0 && points.find(step.top() - 1) != points.end())
	{
		auto a =  points.find(step.top() - 1);
		res = curve_point_add(a->second, a->second);
		points.insert(std::make_pair(step.top(), res));
	}
	else
	{
		auto it = points.rbegin();
		int i;

		i = it->first - 1;
		res = it->second;
		while (++i != step.top())
			res = curve_point_add(res, res);
		points.insert(std::make_pair(step.top(), res));
	}
	return res;
}

ellipse_curve_point ellipse_curve_class::curve_point_mul(ellipse_curve_point &P, int n) const
{
	ellipse_curve_point res;
	std::stack<int> step;
	std::map<int, ellipse_curve_point> points;
	int i = 0;
	
    if (!n || n < 0)
		return res;
	while (n != 0)	// get n in binary
	{
		if (n % 2 == 1)
			step.push(i);
		n = n / 2;
		i++;
	}
	fill_map_with_points(P, step, points); //get map elems
	res = points[(step.top())];
    while (!step.empty())		//add all elems of map
	{
		step.pop();
		if (step.empty())
			break;
        res = curve_point_add(res, points[step.top()]);
	}
	return (res);
}

// check if  y^2 = x^3 + a * x + b (mod m)
bool ellipse_curve_class::if_point_on_curve(ellipse_curve_point &P) const
{
	BigInteger y_equation;
	BigInteger x_equation;

	y_equation = kar_mul(P.y, P.y) % curve.m;
	x_equation = (P.x.toPow(3) + kar_mul(curve.a , P.x) + curve.b) % curve.m;

	return y_equation == x_equation;
}

// check if 4*a^3 + 27*b^2 ≠ 0 (mod m)
bool ellipse_curve_class::if_curve_sutable() const
{
	BigInteger a;
	BigInteger b;
	BigInteger res;

	a = kar_mul(BigInteger(4), curve.a.toPow(3));
	b = kar_mul(BigInteger(27), kar_mul(curve.b, curve.b));
	res = (a + b) % curve.m;

	return res != BigInteger(0);
}

/*	Algo definition:

    n = max(размер X, размер Y)
    если n = 1: вернуть X * Y
    X_l = левые n/2 цифр X
    X_r = правые n/2 цифр X
    Y_l = левые n/2 цифр Y
    Y_r = правые n/2 цифр Y
    Prod1 = kar_mul(X_l, Y_l)
    Prod2 = kar_mul(X_r, Y_r)
    Prod3 = kar_mul(X_l + X_r, Y_l + Y_r)
    вернуть Prod1 * 10 ^ n + (Prod3 - Prod1 - Prod2) * 10 ^ (n / 2) + Prod2
*/

BigInteger ellipse_curve_class::kar_mul(BigInteger P, BigInteger n) const
{
    BigInteger res;
    int div = 1;
    int sign = 1;
    unsigned int len;

    if (P.getSign() == -1 || n.getSign() == -1)
    {
        sign = -1;
        if (P.getSign() == -1 && n.getSign() == -1)
        {
        	P = -P;
        	n = -n;
        	sign = 1;
        }
        else if (P.getSign() == -1)
        	P = -P;
        else if (n.getSign() == -1)
        	n = -n;
    }
    len = (bigIntegerToString(n)).length() > (bigIntegerToString(P)).length() ? (bigIntegerToString(n)).length() : (bigIntegerToString(P)).length();
    if (len == 1)
    {
        res = P * n;
        return (res * sign);
    }
    if ((bigIntegerToString(n)).length()%2)
    {
        n *= BigInteger(10);
        if (len % 2)
        	len++;
        div *= 10;
    }
    if ((bigIntegerToString(P)).length()%2)
    {
        P *= BigInteger(10);
        if (len % 2)
        	len++;
        div *= 10;
    }
    BigInteger divider(BigInteger(10).toPow(len/2));
    BigInteger X_l = P / divider;
    BigInteger X_r = P % divider;
    BigInteger Y_l = n / divider;
    BigInteger Y_r = n % divider;
    BigInteger r1 = kar_mul(X_l , Y_l);
    BigInteger r2 = kar_mul(X_r , Y_r);
    BigInteger ar1(X_l + X_r);
    BigInteger ar2(Y_l + Y_r);
    BigInteger r3 = kar_mul(ar1 , ar2);
    res = r1 * BigInteger(10).toPow(len) + (r3 - r1 - r2) * BigInteger(10).toPow(len / 2) + r2;
    return (res / BigInteger(div)) * BigInteger(sign) ;
}

ellipse_curve ellipse_curve_class::get_curve() const
{
	return curve;
}
