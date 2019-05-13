#ifndef LONGARG_HPP
#define LONGARG_HPP

#include <iostream>
#include <string>
#include <climits>
const int rozrad = 5;
/*
NB! Little-endian format.
*/
typedef uint32_t BNumber [rozrad];
class Longarg
{
    public:
    Longarg();
    void print() const;
    Longarg(const std::string num);
    Longarg(int64_t num);
    Longarg(const Longarg &arg);
    Longarg operator+(const Longarg &arg);
    Longarg operator+(uint64_t arg);
    Longarg operator-(const Longarg &arg);
    Longarg &operator=(const Longarg &arg);
    Longarg operator*(uint64_t arg);
    Longarg &operator+=(const Longarg &arg);
    Longarg operator/(const Longarg &arg);
    Longarg operator*(const Longarg &arg);
    ~Longarg();
    /*Longarg operator%(const Longarg &arg);
    Longarg operator+=(const Longarg &arg);
    Longarg operator-=(const Longarg &arg);
    Longarg operator*=(const Longarg &arg);
    Longarg operator/=(const Longarg &arg);
    Longarg operator%=(const Longarg &arg);
    Longarg operator=(const Longarg &arg);
    Longarg operator==(const Longarg &arg);
    Longarg operator!=(const Longarg &arg);
    Longarg operator>(const Longarg &arg);
    Longarg operator<(const Longarg &arg);
    Longarg operator>=(const Longarg &arg);
    Longarg operator<=(const Longarg &arg);
    Longarg operator "" _p(int pow);

    std::string to_power(int pow);
    void correct();*/


    private:
    BNumber num;
    int num_of_occupied = 0;
    bool sign = 0; // 1 - negative
};

Longarg::Longarg()
{
    //for(int i = 0; i < rozrad; i++)
    //    num[i] = 0;
    //sign = 0;
}

Longarg::Longarg(const std::string num)
{

}
Longarg::Longarg(int64_t number)
{
    int i = 0;
    
    if (number < 0)
        sign = 1;
    for(int i = 0; i < rozrad; i++)
        num[i] = 0;
    num[1] = number / UINT_MAX;
    if (num[1])
        num_of_occupied++;
    num[0] = number % UINT_MAX;
    num_of_occupied++;
}
Longarg::~Longarg()
{}
Longarg::Longarg(const Longarg &arg)
{

}
Longarg Longarg::operator+(uint64_t arg)
{
    Longarg tmp(arg);
    tmp = tmp + *this;
    return tmp;
}
Longarg Longarg::operator+(const Longarg &arg)
{
    Longarg tmp;
    uint64_t carry = 0;
    int bigger_rozrad = num_of_occupied > arg.num_of_occupied ? num_of_occupied : arg.num_of_occupied;
    for (int i = 0; i < rozrad; i++)
        tmp.num[i] = carry =  ((unsigned int)this->num[i] + arg.num[i] +(carry >> UINT_MAX));
        if ((carry >> UINT_MAX) != 0)
            tmp.num_of_occupied++;
    tmp.num_of_occupied += bigger_rozrad;
    return tmp;
}
Longarg Longarg::operator-(const Longarg &arg)
{
    Longarg tmp;
    int bigger_rozrad = num_of_occupied > arg.num_of_occupied ? num_of_occupied : arg.num_of_occupied;
    for (int i = 0; i < bigger_rozrad; i++)
    {
        if (num[i] < arg.num[i])
        {
            num[i+1] ? num[i+1]-- : sign = 1;
            tmp.num[i] = UINT_MAX - arg.num[i] + num[i];
        }
        else
            tmp.num[i] = num[i] - arg.num[i];
    }
    return tmp;
}

Longarg Longarg::operator*(uint64_t arg)
{
    Longarg tmp;
    
    tmp = *this;
    for(int i = 0; i < arg - 1; i++)
        tmp = tmp + *this;
    return tmp;
}

Longarg Longarg::operator*(const Longarg &arg)
{
    Longarg tmp;
    
    tmp = *this;
    unsigned int a =  arg.num[1];
    while (a != 0)
    {
        for(int i = 0; i < UINT_MAX - 1; i++)
            tmp = tmp + *this;
        a--;
    }
    a =  arg.num[0];
    while (a != 0)
    {
        for(int i = 0; i < a - 1; i++)
            tmp = tmp + *this;
       a = 0;
    }
    return tmp;
}

Longarg &Longarg::operator=(const Longarg &arg)
{
    if (this == &arg)
        return (*this);
    for(int i = 0; i < rozrad; i++)
        this->num[i] = arg.num[i];
    sign = arg.sign;
    num_of_occupied = arg.num_of_occupied;
    return (*this);
}

Longarg &Longarg::operator+=(const Longarg &arg)
{
    uint64_t carry = 0;
    int bigger_rozrad = num_of_occupied > arg.num_of_occupied ? num_of_occupied : arg.num_of_occupied;
    for(int i = 0; i < bigger_rozrad; i++)
        this->num[i] = carry =  ((unsigned int)this->num[i] + arg.num[i] +(carry >> UINT_MAX));
    return (*this);
}
Longarg Longarg::operator/(const Longarg &arg)
{
    Longarg tmp;
    int bigger_rozrad = num_of_occupied > arg.num_of_occupied ? num_of_occupied : arg.num_of_occupied;
    if (num_of_occupied == 1 && arg.num_of_occupied == 1)
    {
        tmp.num[0] = num[0] / arg.num[0];
        tmp.num_of_occupied++;
    }
    else
    {
        for (int i = bigger_rozrad - 1; i >= 0; i--)
        {   
            int j = num[i];
            while (j)
            {
                tmp = tmp + UINT_MAX / arg.num[0];
                j--;
            }
        }
    }
    return tmp;
}
void Longarg::print() const
{
    if (!num_of_occupied)
        std::cout << 0 << std::endl;
    else
    {
        if (sign)
            std::cout << "-";
        for(int i = 0; i <= num_of_occupied - 1; i++)
            std::cout << "[" << num[i] << "] ";
        std::cout << std::endl;
    } 
}
/*
class EllipseCurvesClass : public Longarg
{

};
*/
#endif