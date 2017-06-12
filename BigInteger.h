/*
BigInteger.h: Header file for BigInteger structure
By Anthony Enem
*/

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include<string.h>

class BigInteger
{
	typedef long long int BaseType;
	enum COMPARE_RESULT {GREATER = 1, LESS = -1, EQUAL = 0};
	enum SIGN {POSITIVE = 1, NEGATIVE = -1};

private:
	std::vector<BaseType> m_Mag;
	SIGN m_sign;

	/*PRIVATE HELPER METHODS*/

	//Add BigIntegers
	void sum_Magnitudes(const BigInteger&, BigInteger&) const;

	//Multiply BigIntegers
	void multiply(const BigInteger&, BigInteger&) const;

	//Compare BigIntegers
	COMPARE_RESULT compare(const BigInteger&) const;

	//Subtract BigIntegers
	void diff_Magnitudes(const BigInteger&, BigInteger&, COMPARE_RESULT) const;

	COMPARE_RESULT comp_Magnitudes(const BigInteger&) const;

public:
	const int DIGITS_PER_INDEX = 9;
	const BaseType MOD_VAL = pow(10, DIGITS_PER_INDEX);

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
	bool operator!=(const BigInteger&) const;

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
