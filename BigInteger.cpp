/*
BigInteger.cpp: Implementation file for BigInter structure
By Anthony Enem
*/

#include "BigInteger.h"

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

BigInteger::BigInteger(const char* rhs) : 
	DIGITS_PER_INDEX(9), MOD_VAL(pow(10, DIGITS_PER_INDEX))
{
	if (!rhs) {
		N.resize(1); sign = 1;
		return;
	}
	size_t len_rhs = strlen(rhs), copied_len = 0;
	char* digits = new char[DIGITS_PER_INDEX+1];
	ll int_val;

	//assign BigInteger sign
	if (len_rhs && !isdigit(rhs[0])) {
		sign = rhs[0] == '-' ? -1 : 1;
		copied_len = 1;
	}

	//parse digits
	while (copied_len < len_rhs) {
		strncpy(digits, rhs + copied_len, DIGITS_PER_INDEX);
		int_val = std::stoll(digits);
		copied_len += DIGITS_PER_INDEX;
		N.push_back(int_val);
	}
	//vector must have a value
	if (N.size() == 0) {
		N.resize(1); sign = 1;
	}
	reverse(N.begin(), N.end());	
}

BigInteger::BigInteger(ll integer) :
	DIGITS_PER_INDEX(9), MOD_VAL(pow(10, DIGITS_PER_INDEX))
{
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
	BigInteger *result = new BigInteger();
	result->N.resize(std::max(N.size(), other.N.size()));

	ll firstVal, secondVal, sumVal;
	int thisIdx, otherIdx, resultIdx, carry = 0;

	for (thisIdx = 0, otherIdx = 0, resultIdx = 0;
		thisIdx < N.size() || otherIdx < other.N.size(); )
	{
		firstVal = secondVal = 0;

		if (thisIdx < N.size()) {
			firstVal = N[thisIdx];
			++thisIdx;
		}
		if (otherIdx < other.N.size()) {
			secondVal = other.N[otherIdx];
			++otherIdx;
		}
		sumVal = firstVal + secondVal + carry;
		result->N[resultIdx] = sumVal % MOD_VAL;
		carry = (sumVal - result->N[resultIdx]) / MOD_VAL;

		++resultIdx;
	}

	//if carry exists, add to BigInteger
	if (carry) {
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
	int index, lastNonZeroIdx = -1;

	for (index = 0; index < N.size(); index++) {
		if (index < other.N.size()) {
			if (N[index] + carry < other.N[index]) {
				result->N[index] = MOD_VAL + N[index] + carry - other.N[index];
				carry = -1;
			}
			else {
				result->N[index] = N[index] + carry - other.N[index];
				carry = 0;
			}
		}
		else {
			if (N[index] + carry < 0) {
				result->N[index] = MOD_VAL - 1;
				carry = -1;
			}
			else {
				result->N[index] = N[index] + carry;
				carry = 0;
			}
		}
		if (result->N[index] != 0) {
			lastNonZeroIdx = index;
		}
	}

	result->N.resize(std::max(lastNonZeroIdx + 1, 1));

	return *result;
}

//Multiply BigIntegers
BigInteger& BigInteger::multiply(const BigInteger& other) const
{
	BigInteger zero;

	//if either is zero, return zero
	if (other == zero || (*this) == zero)
		return zero;

	BigInteger *result = new BigInteger();
	ll current_value;
	int otherIdx = 0, carry = 0, thisIdx, result_index;

	//loop through second BigInt
	for (; otherIdx < other.N.size(); ++otherIdx)
	{
		result_index = otherIdx;
		//loop through first BigInt
		for (thisIdx = 0; thisIdx < N.size(); thisIdx++, ++result_index) {
			//multiply value at index of second to index of first, add carry from previous calculation
			current_value = (other.N[otherIdx] * N[thisIdx]) + carry;

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
BigInteger& BigInteger::operator *(const BigInteger& other) const
{
	return multiply(other);
}

BigInteger& BigInteger::operator*=(const BigInteger& other)
{
	(*this) = multiply(other);

	return *this;
}

//Greater than
bool BigInteger::operator>(const BigInteger& other) const
{
	if (compare(other) == 1) {
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
bool BigInteger::operator<(const BigInteger& other) const
{
	if (compare(other) == -1) {
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
BigInteger& operator+(const BigInteger& other) const;
BigInteger& operator+=(const BigInteger& other);

//Subtraction
BigInteger& operator-(const BigInteger& other) const;
BigInteger& operator-=(const BigInteger& other);
*/

std::ostream& operator<<(std::ostream& os, const BigInteger &bg)
{
	if (bg.sign == -1) {
		os << '-';
	}
	os << bg.N[bg.N.size() - 1];
	for (int i = bg.N.size() - 2; i > -1; i--)
	{
		os << std::setfill('0') << std::setw(bg.DIGITS_PER_INDEX) << bg.N[i];
	}
	return os;
}