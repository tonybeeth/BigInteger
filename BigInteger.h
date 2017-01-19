/*
BigInteger.h: Header file for BigInteger structure
By Anthony Enem
*/

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>

class BigInteger
{
	typedef long long int BaseType;
private:
	std::vector<BaseType> m_Mag;
	int m_sign;

	/*PRIVATE HELPER METHODS*/

	//Add BigIntegers
	void sum_Magnitudes(const BigInteger&, BigInteger&) const;

	//Multiply BigIntegers
	void multiply(const BigInteger&, BigInteger&) const;

	//Compare BigIntegers
	int compare(const BigInteger&) const;

	//Subtract BigIntegers
	void diff_Magnitudes(const BigInteger&, BigInteger&, int) const;

	int comp_Magnitudes(const BigInteger&) const;

public:
	static const int DIGITS_PER_INDEX = 9;
	static const BaseType MOD_VAL = pow(10, DIGITS_PER_INDEX);

	//default constructor
	BigInteger();

	BigInteger(BaseType integer);

	BigInteger(const char*);

	//copy constructor
	BigInteger(const BigInteger&);

	//Destructor
	~BigInteger();

	bool isNegative() const;

	BigInteger Abs() const;

	bool isZero() const;

	/*OVERLOADED OPERATORS*/

	//Addition
	BigInteger operator+(const BigInteger&) const;
	BigInteger& operator+=(const BigInteger&);

	//Negate operator
	BigInteger operator-(void) const;

	//Subtraction
	BigInteger operator-(const BigInteger&) const;
	BigInteger& operator-=(const BigInteger&);

	//Multiplication
	BigInteger operator *(const BigInteger&) const;
	BigInteger& operator*=(const BigInteger&);

	//Greater than
	bool operator>(const BigInteger&) const;
	bool operator >=(const BigInteger&) const;

	//Less than
	bool operator<(const BigInteger&) const;
	bool operator<=(const BigInteger&) const;

	//Equals
	bool operator==(const BigInteger&) const;

	//Assignment operator
	BigInteger& operator=(const BigInteger&);

	friend std::ostream& operator<<(std::ostream&, const BigInteger&);

	//Increment
	BigInteger& operator++(); //prefix
	BigInteger operator++(int); //postfix

	//Decrement
	BigInteger& operator--();
	BigInteger operator--(int);

};


#endif // !BIG_INTEGER_H
