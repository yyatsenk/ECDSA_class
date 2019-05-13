#include "Longarg.hpp"
#include <math.h>

int phi(int n) {
	int ret = 1;
	for(int i = 2; i * i <= n; ++i) {
		int p = 1;
		while(n % i == 0) {
			p *= i;
			n /= i;
		}
		if((p /= i) >= 1) ret *= p * (i - 1);
	}
	return --n ? n * ret : ret;
}
int main()
{
    Longarg t1(4294967300);
    Longarg t2(50);
    //a.print();
    //b.print();

      
     int a =1;
     int b =1;
     int m = 5;
     int x1(0);
     int x2(4);
     int y1(1);
     int y2(2);

      int res_x;
      int res_y;
      int lambda;

      printf("phi %d\n", phi((x2- x1)));
      if ((x2- x1) > (y2-y1))
            lambda = (y2-y1) * (int)pow((x2- x1), phi((x2- x1)) - 1)%m;
      else
            lambda = (y2-y1) / (x2- x1) % m;
      res_x = (int)(pow(lambda, 2) - x1 -x2)%m;
      int rrr = -(lambda *( x1 - res_x) - y1);
      int i =2;
      while (m < rrr)
       {
             m = m*i;
             i++;
       } 
       printf("rrr %d\n", rrr);
      res_y =m - rrr;

      printf("x = %d y = %d\n", res_x, res_y);

    t1 = t2*t2;
    t1.print();
    return (0);
}