/*
BigInteger.cpp: Implementation file for BigInter structure
By Anthony Enem
*/

#include "BigInteger.h"

//Default constructor
BigInteger::BigInteger()
{
	N.resize(1);
	sign = 1;
}

//Destructor
BigInteger::~BigInteger()
{
	N.clear();
}

//string parameterized constructor
BigInteger::BigInteger(const char* rhs)
{
	if (!rhs) {
		*this = +0;
		return;
	}
	size_t len_rhs = strlen(rhs), copied_len = 0;
	char* digits = new char[DIGITS_PER_INDEX + 1];
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

//Parameterized constructor for integers
BigInteger::BigInteger(ll integer)
{
	this->sign = integer < 0 ? -1 : 1;
	if(integer == 0){
		N.resize(1);
		return;
	}
	//Split integer into vector
	integer = abs(integer);
	while (integer > 0)
	{
		N.push_back(integer % MOD_VAL);
		integer /= MOD_VAL;
	}
}

//copy constructor
BigInteger::BigInteger(const BigInteger& other)
{
	N.clear();
	N.resize(other.N.size());
	N = other.N;
	sign = other.sign;
}

/*PRIVATE HELPER METHODS*/

//Get sum of BigInteger magnitudes
void BigInteger::sum_Magnitudes(const BigInteger& other, BigInteger& result) const
{
	result.N.resize(std::max(N.size(), other.N.size()));

	ll firstVal, secondVal, sumVal;
	int idx, carry = 0;
	
	for (idx = 0; idx < result.N.size(); ++idx)
	{
		firstVal = secondVal = 0;

		if (idx < N.size()) {
			firstVal = N[idx];
		}
		if (idx < other.N.size()) {
			secondVal = other.N[idx];
		}
		sumVal = firstVal + secondVal + carry;
		result.N[idx] = sumVal % MOD_VAL;
		carry = sumVal / MOD_VAL;
	}

	//if carry exists, add to BigInteger
	if (carry) {
		result.N.push_back(carry);
	}
}

//Get difference of BigInteger Magnitudes
void BigInteger::diff_Magnitudes(const BigInteger& other, BigInteger& result, int lhs_greater) const
{
	int8_t carry = 0;
	const BigInteger *first = this, *second = &other;

	//assign first pointer to BigInteger with bigger Magnitude, second pointer to smaller
	if (lhs_greater == -1) {
		first = &other;
		second = this;
	}

	result.N.resize(std::max(N.size(), other.N.size()));
	int index, lastNonZeroIdx = -1;
	ll first_val, second_val;

	for (index = 0; index < first->N.size(); ++index) {
		first_val = first->N[index] + carry;
		second_val = 0;
		//assign second value if it exists
		if (index < second->N.size()) {
			second_val = second->N[index];
		}
		//calculate result and assign appropriate carry
		if (first_val >= second_val) {
			result.N[index] = first_val - second_val;
			carry = 0;
		}
		else {
			result.N[index] = MOD_VAL + first_val - second_val;
			carry = -1;
		}

		if (result.N[index] != 0) {
			lastNonZeroIdx = index;
		}
	}
	//shrink result's magnitude to exclude leading zeros
	result.N.resize(std::max(lastNonZeroIdx + 1, 1));
}

//Multiply BigIntegers
void BigInteger::multiply(const BigInteger& other, BigInteger& result) const
{
	//if either multiplicand or multiplier is zero, return zero
	if (this->isZero() || other.isZero()) {
		//set result to zero
		result = +0;
		return;
	}

	//allocate at least the bigger size of both BigIntegers for the result
	result.N.resize(std::max(N.size(), other.N.size()));
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

			if (result_index < result.N.size()) { //if result fits
				current_value += result.N[result_index];
				result.N[result_index] = (current_value % MOD_VAL);
			}
			else
				result.N.push_back(current_value % MOD_VAL); //need to expand result
			carry = current_value / MOD_VAL; //compute new carry
		}
		//if carry is left, expand result
		if (carry) {
			result.N.push_back(carry);
			carry = 0;
		}
	}

	result.sign = sign * other.sign;
}

//Compare BigIntegers
//0 for equal, 1 for greater, -1 for less
int BigInteger::compare(const BigInteger& other) const
{
	//Check signs
	if (sign > other.sign)
		return 1;
	if (other.sign > sign)
		return -1;
	//First check vector sizes
	if (N.size() > other.N.size())
		return 1;
	if (N.size() < other.N.size())
		return -1;

	//BigIntegers have same signs and vector size. check Magnitudes
	return comp_Magnitudes(other);
}

//Compare just magnitudes of both BigIntegers
int BigInteger::comp_Magnitudes(const BigInteger& other) const
{
	for (int i = N.size() - 1; i != -1; --i)
	{
		if (N[i]*sign > other.N[i]*other.sign)
			return 1;
		if (N[i]*sign < other.N[i]*other.sign)
			return -1;
	}
	return 0;
}

/*OVERLOADED OPERATORS*/

//Multiplication
BigInteger BigInteger::operator *(const BigInteger& other) const
{
	BigInteger result;
	multiply(other, result);
	return result;
}

BigInteger& BigInteger::operator*=(const BigInteger& other)
{
	BigInteger result;
	multiply(other, result);
	*this = std::move(result);

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
BigInteger BigInteger::operator-(void) const
{
	BigInteger result = *this;
	result.sign *= -1;
	return result;
}

bool BigInteger::isNegative() const
{
	return sign == -1;
}

BigInteger BigInteger::Abs() const
{
	BigInteger result = *this;
	result.sign = 1;
	return result;
}

//Equivalent to BigInteger == 0 to be used for special cases
bool BigInteger::isZero() const
{
	return N.size() == 1 && N[0] == 0;
}

//Addition operator
BigInteger BigInteger::operator+(const BigInteger& other) const
{
	BigInteger result;
	//Get Magnitude comparison result
	int comp_Mag = comp_Magnitudes(other);
	//if both are of the same signs, sum their magnitudes
	if (sign == other.sign) {
		sum_Magnitudes(other, result);
		result.sign = sign;
	}
	else {
		//get Magnitude difference, assign appropriate sign
		diff_Magnitudes(other, result, comp_Mag);
		//if negative BigInteger has bigger magnitude, result's sign is negative, else positive
		//if((sign == -1 && comp_Mag == 1) || (other.sign == -1 && comp_Mag == -1))

		result.sign = (sign * comp_Mag) | 1;
	}
	return result;
}

BigInteger& BigInteger::operator+=(const BigInteger& other)
{
	*this = std::move(this->operator+(other));
	return *this;
}

//Subtraction
BigInteger BigInteger::operator-(const BigInteger& other) const
{
	BigInteger result;
	int comp_Mag = comp_Magnitudes(other);
	//if both BigIntegers have opposite signs, sum their magnitudes. resulting sign
	//is sign of lhs
	if (this->sign != other.sign) {
		sum_Magnitudes(other, result);
		result.sign = sign;
	}
	else {
		//get difference of magnitudes and assign appropriate sign
		diff_Magnitudes(other, result, comp_Mag);
		//if ((this->isNegative() && comp_Mag == 1) || (other.isNegative() && comp_Mag == -1))
		result.sign = (sign * comp_Mag) | 1;
	}
	return result;
}

BigInteger& BigInteger::operator-=(const BigInteger& other)
{
	*this = std::move(this->operator-(other));
	return *this;
}

//Increment
BigInteger& BigInteger::operator++() //prefix
{
	*this += 1;
	return *this;
}

BigInteger BigInteger::operator++(int) //postfix
{
	BigInteger temp = *this;
	*this += 1;
	return temp;
}

//Decrement
BigInteger& BigInteger::operator--()
{
	*this -= 1;
	return *this;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger temp = *this;
	*this -= 1;
	return temp;
}

//Overloaded insertion operator for ostream
std::ostream& operator<<(std::ostream& os, const BigInteger &bg)
{
	if (bg.sign == -1) {
		os << '-';
	}
	os << bg.N.back();
	for_each(bg.N.rbegin() + 1, bg.N.rend(), [&bg, &os](const ll& val){
		os << std::setfill('0') << std::setw(bg.DIGITS_PER_INDEX) << val;
	});
	return os;
}