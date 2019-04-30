#include "Longarg.hpp"

int main()
{
    Longarg a(211);
    Longarg b(199);
    a.print();
    b.print();
    b = a - b;
    b.print();
    return (0);
}