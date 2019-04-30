#ifndef LONGARG_HPP
#define LONGARG_HPP

#include <iostream>
#include <string>
const int rozrad = 5;
/*
NB! Little-endian format.
*/
typedef char BNumber [rozrad];
class Longarg
{
    public:
    Longarg();
    void print() const;
    Longarg(const std::string num);
    Longarg(int64_t num);
    Longarg(const Longarg &arg);
    Longarg operator+(const Longarg &arg);
    Longarg operator-(const Longarg &arg);
    Longarg &operator=(const Longarg &arg);
    /*Longarg operator*(const Longarg &arg);
    Longarg operator/(const Longarg &arg);
    Longarg operator%(const Longarg &arg);
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

    virtual ~Longarg();
    std::string to_power(int pow);
    void correct();*/


    private:
    BNumber num;
    bool sign = 0; // 1 - negative
};

Longarg::Longarg()
{
    for(int i = 0; i < rozrad; i++)
        num[i] = 0;
    sign = 0;
}

Longarg::Longarg(const std::string num)
{

}
Longarg::Longarg(int64_t number)
{
    int num_len = 1;
    int64_t number_copy = number;
    int i = 0;
    
    if (number < 0)
        sign = 1;
    for(int i = 0; i < rozrad; i++)
        num[i] = 0;
    while (number_copy /= 10)
        num_len++;
    while (i != num_len)
    {
        num[i] = number % 10;
        i++;
        number /= 10;
    }
}

Longarg::Longarg(const Longarg &arg)
{

}
Longarg Longarg::operator+(const Longarg &arg)
{
    Longarg tmp;

    for (int i = 0; i < rozrad; i++)
        if (num[i] + arg.num[i] > 9)
        {
            tmp.num[i] += (num[i] + arg.num[i]) % 10;
            if (i + 1 == rozrad)
            {
                std::cout << "Overflow happened\n";
                break;
            }
            tmp.num[i + 1]++;
        }
        else
           tmp.num[i] += num[i] + arg.num[i]; 
    return tmp;
}
Longarg Longarg::operator-(const Longarg &arg)
{
    Longarg tmp;
    for (int i = rozrad - 1; i >= 0; i--)
        if (num[i] - arg.num[i] < 0)
        {
            tmp.num[i] += (num[i] - arg.num[i] + 10);
            int j = i;
            while (j > 0 && !num[j - 1])
                j--;
            if (i != 0)
                num[j - 1]--;
        }
        else
           tmp.num[i] -= num[i] - arg.num[i]; 
    return tmp;
}
Longarg &Longarg::operator=(const Longarg &arg)
{
    if (this == &arg)
        return (*this);
    for(int i = 0; i < rozrad; i++)
        this->num[i] = arg.num[i];
    return (*this);
}

void Longarg::print() const
{
    bool null_value = 1;
    int start;
    for(start = rozrad  - 1; start >= 0; start--)
        if (num[start] != 0)
        {
            null_value = 0;
            break;
        }
    if (null_value)
        std::cout << 0;
    else
        for(int i = start; i >= 0; i--)
            std::cout << (int)num[i];
    std::cout << std::endl;
}
/*
class EllipseCurvesClass : public Longarg
{

};
*/
#endif