#include<iostream>
#include "BigInteger.h"

using namespace std;

int main()
{
	BigInteger y = 1;

	//Calculating 1000 factorial
	for(BigInteger x = 2; x <= 1000; ++x){
		y *= x;
	}

	cout << y << endl;
	return 0;
}
