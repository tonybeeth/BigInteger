/*
BigInteger.cpp: Implementation file for BigInter structure
By Anthony Enem
*/

#include "BigInteger.h"
#include <math.h>
#include <algorithm>

/*
Default Constructor
BigInteger defaults to a value of 0 and positive
*/
BigInteger::BigInteger() :
	DIGITS_PER_INDEX(9), MOD_VAL(pow(10, DIGITS_PER_INDEX))
{
	N.resize(1);
	sign = 1;
}

BigInteger::~BigInteger()
{
	N.clear();
}

BigInteger::BigInteger(ll integer) :
	DIGITS_PER_INDEX(9), MOD_VAL(pow(10, DIGITS_PER_INDEX))
{
	sign = 1;
	N.push_back(integer);
}

//copy constructor
BigInteger::BigInteger(const BigInteger& other) :
	DIGITS_PER_INDEX(other.DIGITS_PER_INDEX), MOD_VAL(other.MOD_VAL)
{
	N.clear();
	N.resize(other.N.size());
	N = other.N;
	sign = other.sign;
}


/*PRIVATE HELPER METHODS*/

//Add BigIntegers
BigInteger& BigInteger::add(const BigInteger& other) const
{
	int i = 0, minLen = min(N.size(), other.N.size()), maxLen = max(N.size(), other.N.size());

	BigInteger *result = new BigInteger();
	result->N.resize(maxLen);

	ull current_value, carry = 0;
	for (; i < minLen; ++i)	{
		current_value = N[i] + other.N[i] + carry;
		result->N[i] = current_value % MOD_VAL;
		carry = (current_value - result->N[i]) / MOD_VAL;
	}
	//continue with values of larger BigInteger
	if (i == N.size())
	{
		for (; i < other.N.size(); ++i)
		{
			current_value = other.N[i] + carry;
			result->N[i] = current_value % MOD_VAL;
			carry = (current_value - result->N[i]) / MOD_VAL;
		}
	}
	else
	{
		for (; i < N.size(); ++i)
		{
			current_value = N[i] + carry;
			result->N[i] = current_value % MOD_VAL;
			carry = (current_value - result->N[i]) / MOD_VAL;
		}
	}
	//if carry exists, add to BigInteger
	if (carry)
	{
		result->N.push_back(carry);
	}

	return *result;
}

BigInteger& BigInteger::subtract(const BigInteger& other) const
{
	BigInteger *result = new BigInteger();
	int8_t carry = 0;
	result->N.clear();

	result->N.resize(N.size());
	int index;

	for (index = 0; index < N.size(); index++) {
		if (index < other.N.size()) {
			if (ll(N[index]) + carry < ll(other.N[index])) {
				result->N[index] = MOD_VAL + N[index] + carry - other.N[index];
				carry = -1;
			}
			else {
				result->N[index] = N[index] + carry - other.N[index];
				carry = 0;
			}
		}
		else {
			if (ll(N[index]) + carry < 0) {
				result->N[index] = MOD_VAL - 1;
				carry = -1;
			}
			else {
				result->N[index] = N[index] + carry;
				carry = 0;
			}
		}
	}

	index = result->N.size();
	while (index >= 0 && result->N[--index] == 0);
	result->N.resize(max(index + 1, 1));

	return *result;
}

//Multiply BigIntegers
BigInteger& BigInteger::multiply(const BigInteger& other) const
{
	BigInteger zero = 0;

	//if either is zero, return zero
	if (other == zero || (*this) == zero)
		return zero;

	BigInteger *result = new BigInteger();
	ll current_value;
	int other_index = 0, carry = 0, this_index, result_index;

	//loop through second BigInt
	for (; other_index < other.N.size(); ++other_index)
	{
		result_index = other_index;
		//loop through first BigInt
		for (this_index = 0; this_index < N.size(); this_index++, ++result_index) {
			//multiply value at index of second to index of first, add carry from previous calculation
			current_value = (other.N[other_index] * N[this_index]) + carry;

			if (result_index < result->N.size()) { //if result fits
				current_value += result->N[result_index];
				result->N[result_index] = (current_value % MOD_VAL);
			}
			else
				result->N.push_back(current_value % MOD_VAL); //need to expand result
			carry = current_value / MOD_VAL; //compute new carry
		}
		//if carry is left, expand result
		if (carry) {
			result->N.push_back(carry);
			carry = 0;
		}
	}

	result->sign = sign * other.sign;
	return *result;
}

//Compare BigIntegers
//0 for equal, 1 for greater, -1 for less
int BigInteger::compare(const BigInteger& other) const
{
	//First check the vector sizes
	if (N.size() > other.N.size())
		return 1;
	if (N.size() < other.N.size())
		return -1;

	//vectors have same size. check individual indices
	for (int i = N.size() - 1; i != -1; --i)
	{
		if (N[i] > other.N[i])
			return 1;
		if (N[i] < other.N[i])
			return -1;
	}

	return 0;
}

/*OVERLOADED OPERATORS*/

//Multiplication
BigInteger& BigInteger::operator *(ll integer) const
{
	return multiply(BigInteger(integer));
}

BigInteger& BigInteger::operator *(const BigInteger& other) const
{
	return multiply(other);
}

BigInteger& BigInteger::operator*=(ll integer)
{
	(*this) = operator*(integer);

	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other)
{
	(*this) = multiply(other);

	return *this;
}

//Greater than
bool BigInteger::operator>(ll integer) const
{
	if (compare(BigInteger(integer)) == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator>(const BigInteger& other) const
{
	if (compare(other) == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator >=(ll integer) const
{
	if (compare(BigInteger(integer)) != -1) {
		return true;
	}
	else {
		return false;
	}
}


bool BigInteger::operator >=(const BigInteger& other) const
{
	if (compare(other) != -1) {
		return true;
	}
	else {
		return false;
	}
}

//Less than
bool BigInteger::operator<(ll integer) const
{
	if (compare(BigInteger(integer)) == -1) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator<(const BigInteger& other) const
{
	if (compare(other) == -1) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator<=(ll integer) const
{
	if (compare(BigInteger(integer)) != 1) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator<=(const BigInteger& other) const
{
	if (compare(other) != 1) {
		return true;
	}
	else {
		return false;
	}
}

//Equals
bool BigInteger::operator==(ll integer) const
{
	if (compare(BigInteger(integer)) == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool BigInteger::operator==(const BigInteger& other) const
{
	if (compare(other) == 0) {
		return true;
	}
	else {
		return false;
	}
}

//Assignment operator
BigInteger& BigInteger::operator=(ll integer)
{
	N.clear();
	//Split integer into vector
	while (integer > 0)
	{
		N.push_back(integer % MOD_VAL);
		integer /= MOD_VAL;
	}

	//Check sign
	if (integer < 0) {
		sign = -1;
	}
	else {
		sign = 1;
	}
	return *this;
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
	N.clear();
	N.resize(other.N.size());
	N = other.N;
	sign = other.sign;
	
	return *this;
}

//Negate operator
BigInteger& BigInteger::operator-(void)
{
	sign *= -1;
	return *this;
}

BigInteger& BigInteger::operator+(const BigInteger& other) const
{
	return add(other);
}

BigInteger& BigInteger::operator-(const BigInteger& other) const
{
	return subtract(other);
}

/*
//Addition
BigInteger& operator+(ll integer) const;
BigInteger& operator+(const BigInteger& other) const;
BigInteger& operator+=(ll integer);
BigInteger& operator+=(const BigInteger& other);

//Subtraction
BigInteger& operator-(ll integer) const;
BigInteger& operator-(const BigInteger& other) const;
BigInteger& operator-=(ll integer);
BigInteger& operator-=(const BigInteger& other);
*/