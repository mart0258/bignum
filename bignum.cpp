#include "bignum.h"

#include <iostream>

#include <string>

struct testinfo {
	char *first;
	char op;
	char *second;
	char *result;
	bool negative;
} tests[] = {
	{"1", '+', "2", "3", false},
	{"1", '-', "2", "-1", true},
	{"2", '*', "3", "6", false},
	{"8", '/', "4", "2", false},
	{"-8", '/', "4", "-2", true},

	{"4294967295",'+',"1","4294967296", false},
	{"4295032833", '-', "131074", "4294901759", false},
	{"314159", '*', "265358", "83364603922", false},
	{"83364603922", '/', "265358", "314159", false},

};

int runtests()
{
	bignum<8> x,y,z;
	bool fail=false;
	int i;
	for (i=0; i<sizeof(tests)/sizeof(testinfo); ++i)
	{
		if (tests[i].first==NULL)
			break;
		x=tests[i].first;
		y=tests[i].second;
		switch (tests[i].op)
		{
		case '+':
			z=x+y;
			break;
		case '-':
			z=x-y;
			break;
		case '*':
			z=x*y;
			break;
		case '/':
			if (tests[i].negative)
			{
				z=x.divideSigned(y);
			}
			else
			{
				z=x/y;
			}
			break;
		}
		std::string result;
		if (tests[i].negative)
			result = z.toStringSigned();
		else
			result = z.toString();

		if (strcmp(result.c_str(), tests[i].result))
		{
			fail=true;
			printf ("Missed test %s %c %s = %s (Got %s)\n", tests[i].first, tests[i].op, tests[i].second, tests[i].result, result.c_str());
		}
	}

	if (!fail)
		printf ("Tests passed.\n");
	return fail;
}
int main (int argc, char **argv)
{
	bignum<8> x,y,z;
	char op;

	bool sign=false;
	int state=0;
	for (int i=1; i<argc; ++i)
	{
		if (!strncmp(argv[i], "--", 2))
		{
			if (!strcmp(argv[i], "--test"))
			{
				return runtests();
			}

			continue;
		}
		switch (state++)
		{
		case 0:
			x=argv[i];
			if (argv[i][0]=='-' || argv[i][0]=='+')
				sign=true;
			break;
		case 1:
			op=argv[i][0];
			break;
		case 2:
			y=argv[i];
			if (argv[i][0]=='-' || argv[i][0]=='+')
				sign=true;
			break;
		}
	}
	if (state<3)
	{
		printf ("Usage: %s [<left> <op> <right>]", argv[0]);
		return 1;
	}

	switch (op)
	{
	case '+':
		z=x+y;
		break;
	case '-':
		z=x-y;
		break;
	case '*':
		z=x*y;
		break;
	case '/':
		if (sign)
		{
			z=x.divideSigned(y);
		}
		else
		{
			z=x/y;
		}
		break;
	}

	std::string result;
	if (sign)
		result = z.toStringSigned();
	else
		result = z.toString();

	printf ("%s\n", result.c_str());

	return 0; 
}
