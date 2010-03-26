#include <iostream>

#include <string>
#include "bignum.h"


int main (void)
{
	bignum<2> x,y,z;

	x.num[0]=0x0001; x.num[1]=1;
	y.num[0]=0x2; y.num[1]=0;
	printf ("%x %x\n", x.num[1], x.num[0]);
	printf ("%x %x\n", y.num[1], y.num[0]);
	z=x/y;

	//std::cout << (x < y) << std::endl;
/*
	printf ("x:%x %x\n", x.num[1], x.num[0]);
	printf ("y:%x %x\n", y.num[1], y.num[0]);
	printf ("z:%x %x\n", z.num[1], z.num[0]);
	/**/
	
	std::string s = x.toString();

	std::cout << "result:" << s << std::endl;
	/**/
}
