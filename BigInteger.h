
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include<vector>
using namespace std;

#define endl '\n'
#define ull unsigned long long
#define ll long long


class BigInteger
{
private:
	vector<ull> N;
	int sign;

	/*PRIVATE HELPER METHODS*/

	//Subtract BigIntegers
	BigInteger& subtract(const BigInteger& other) const;

	//Add BigIntegers
	BigInteger& add(const BigInteger& other) const;

	//Multiply BigIntegers
	BigInteger& multiply(const BigInteger& other) const;

	//Compare BigIntegers
	int compare(const BigInteger& other) const;

public:
	const int DIGITS_PER_INDEX = 9;
	const ull MOD_VAL = pow(10, DIGITS_PER_INDEX);

	//default constructor
	BigInteger();

	//copy constructor
	BigInteger(const BigInteger& other);


	/*OVERLOADED OPERATORS*/

	//Addition
	BigInteger& operator+(ll integer) const;
	BigInteger& operator+(const BigInteger& other) const;

	//Negate operator
	BigInteger& operator-(void) const;

	//Subtraction
	BigInteger& operator-(ll integer) const;
	BigInteger& operator-(const BigInteger& other) const;

	//Multiplication
	BigInteger& operator *(ll integer) const;
	BigInteger operator *(const BigInteger& other) const;

	//Greater than
	bool operator>(ll integer) const;
	bool operator>(const BigInteger& other) const;

	//Less than
	bool operator<(ll integer) const;
	bool operator<(const BigInteger& other) const;

	//Equals
	bool operator==(ll integer) const;
	bool operator==(const BigInteger& other);

	//Assignment operator
	BigInteger& operator=(ll integer);
	BigInteger& operator=(const BigInteger& other);

};


#endif // !BIG_INTEGER_H
