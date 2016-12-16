/*
BigInteger.h: Header file for BigInteger structure
By Anthony Enem
*/

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include<iostream>
#include<iomanip>
#include<vector>
using namespace std;

#define endl '\n'
#define ll long long

class BigInteger
{
private:
	vector<ll> N;
	int sign;

	/*PRIVATE HELPER METHODS*/

	//Add BigIntegers
	BigInteger& add(const BigInteger& other) const;

	//Multiply BigIntegers
	BigInteger& multiply(const BigInteger& other) const;

	//Compare BigIntegers
	int compare(const BigInteger& other) const;

	//Subtract BigIntegers
	BigInteger& subtract(const BigInteger& other) const;

public:
	const int DIGITS_PER_INDEX;
	const ll MOD_VAL;

	//default constructor
	BigInteger();

	BigInteger(ll integer);

	//copy constructor
	BigInteger(const BigInteger& other);

	//Destructor
	~BigInteger();

	/*OVERLOADED OPERATORS*/

	//Addition
	BigInteger& operator+(const BigInteger& other) const;
	BigInteger& operator+=(const BigInteger& other);

	//Negate operator
	BigInteger& operator-(void);

	//Subtraction
	BigInteger& operator-(const BigInteger& other) const;
	BigInteger& operator-=(const BigInteger& other);

	//Multiplication
	BigInteger& operator *(const BigInteger& other) const;
	BigInteger& operator*=(const BigInteger& other);

	//Greater than
	bool operator>(const BigInteger& other) const;
	bool operator >=(const BigInteger& other) const;

	//Less than
	bool operator<(const BigInteger& other) const;
	bool operator<=(const BigInteger& other) const;

	//Equals
	bool operator==(const BigInteger& other) const;

	//Assignment operator
	BigInteger& operator=(const BigInteger& other);

	friend  ostream& operator<<(ostream& os, const BigInteger &bg);

};


#endif // !BIG_INTEGER_H
