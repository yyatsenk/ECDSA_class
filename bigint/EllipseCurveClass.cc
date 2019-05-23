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
        lambda = (((Q.y - P.y)  * (Q.x - P.x).toPow(phi(curve.m) - 1))) % curve.m;
    else
        lambda = ((((BigInteger(3) * (P.x * P.x)) + curve.a)* (BigInteger(2) * P.y).toPow(phi(curve.m) - 1)))%curve.m;
    res.x = ((lambda * lambda) -P.x - Q.x) % curve.m;
    res.y = ((lambda * (P.x - res.x)) - P.y) % curve.m;

    return (res);
}

ellipse_curve_point ellipse_curve_class::curve_point_mul(ellipse_curve_point &P, int n) const
{
    ellipse_curve_point res;
    std::vector<int> step;
    std::map<int, ellipse_curve_point> points;
    int i = 0;
    int size = 0;
    int start = 0;
	
    if (!n || n < 0)
        return res;
    while (n != 0)
    {
        if (n % 2 == 1)
        {
            size++;
            step.push_back(i);
        }
        n = n / 2;
        i++;
    }
    res = P;
    while (start < size)
    {
        if (step[start] == 0)
        {
            points.insert(std::make_pair(step[start], res));
            start++;
            continue;
        }
        auto point = points.find(step[start] - 1);
        if (point == points.end())
        {
            int start_copy = step[start] - 2;
            while (start_copy != -1)
            {
                auto point_back = points.find(step[start_copy]);
                if (point_back != points.end())
                    break;
                start_copy--;
            }
            if (start_copy != -1)
                res = points[step[start_copy]];
            start_copy = start_copy != -1 ? step[start_copy] : 0;
            while (step[start] != start_copy)
            {
                res = curve_point_add(res, res);
                start_copy++;
            }
            points.insert(std::make_pair(step[start], res));
        }
        else
        {
            res = curve_point_add(point->second, point->second);
            points.insert(std::make_pair(step[start], res));
        }
        start++;
    }
    res = points[*(step.begin())];
    for (auto v = step.begin() + 1 ; v != step.end();  v++)
        res = curve_point_add(res, points[*v]);
    return (res);
}

// check if  y^2 = x^3 + a * x + b (mod m)
bool ellipse_curve_class::if_point_on_curve(ellipse_curve_point &P) const
{
    BigInteger y_equation;
    BigInteger x_equation;

    y_equation = (P.y * P.y) % curve.m;
    x_equation = (P.x.toPow(3) + (curve.a * P.x) + curve.b) % curve.m;

    return y_equation == x_equation;
}

// check if 4*a^3 + 27*b^2 ≠ 0 (mod m)
bool ellipse_curve_class::if_curve_sutable() const
{
    BigInteger a;
    BigInteger b;
    BigInteger res;

    a = (BigInteger(4) * curve.a.toPow(3));
    b = (BigInteger(27) * (curve.b * curve.b));
    res = (a + b) % curve.m;

    return res != BigInteger(0);
}

ellipse_curve ellipse_curve_class::get_curve() const
{
    return curve;
}
