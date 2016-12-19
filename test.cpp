#include<iostream>
#include "BigInteger.h"

using namespace std;

int main()
{
	BigInteger a = "2893094";
	BigInteger b = 2893094;
	BigInteger c = a * b * a * b;

	cout << c << endl;
	return 0;
}
