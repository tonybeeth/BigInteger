/*
BigInteger.cpp: Implementation file for BigInter structure
By Anthony Enem
*/

#include "BigInteger.h"

//Default constructor
BigInteger::BigInteger()
{
	m_Mag.push_back(0);
	m_sign = POSITIVE;
}

//Destructor
BigInteger::~BigInteger()
{
	m_Mag.clear();
}

//char* parameterized constructor
BigInteger::BigInteger(const char* rhsDigits)
{
	if (!rhsDigits || strlen(rhsDigits) == 0) {
		*this = BaseType(0);
		return;
	}

	const char* rhs = isdigit(rhsDigits[0]) ? rhsDigits : rhsDigits + 1;

	size_t len_rhs = strlen(rhs), copied_len = 0;
	char* digits = new char[DIGITS_PER_INDEX + 1];
	BaseType ll_val;

	//assign BigInteger sign
	if (len_rhs && len_rhs) {
		m_sign = rhs[0] == '-' ? NEGATIVE : POSITIVE;
	}

	//parse digits
	while (copied_len + DIGITS_PER_INDEX <= len_rhs) {
		//Copy next set of digits from end of rhs
		strncpy(digits, rhs + len_rhs - (DIGITS_PER_INDEX + copied_len), DIGITS_PER_INDEX);
		//convert to integer
		ll_val = std::stoll(digits);
		copied_len += DIGITS_PER_INDEX;
		//push to magnitude vector
		m_Mag.push_back(ll_val);
	}
	if (copied_len < len_rhs) {
		std::fill(digits, digits + DIGITS_PER_INDEX, '.'); //replace old digits with non numeric characters
		m_Mag.push_back(std::stoll(strncpy(digits, rhs, len_rhs - copied_len))); //Push last set of digits
	}
	//vector must have a value
	if (m_Mag.size() == 0) {
		m_Mag.push_back(0); 
		m_sign = POSITIVE;
	}

	delete[](digits);
}

//Parameterized constructor for integers
BigInteger::BigInteger(BaseType integer)
{
	this->m_sign = integer < 0 ? NEGATIVE : POSITIVE;
	if (integer == 0) {
		m_Mag.push_back(0);
		return;
	}
	//Split integer into vector
	integer = abs(integer);
	while (integer > 0)
	{
		m_Mag.push_back(integer % MOD_VAL);
		integer /= MOD_VAL;
	}
}

//copy constructor
BigInteger::BigInteger(const BigInteger& other)
{
	m_Mag.clear();
	m_Mag.resize(other.m_Mag.size());
	m_Mag = other.m_Mag;
	m_sign = other.m_sign;
}

/*PRIVATE HELPER METHODS*/

//Get sum of BigInteger magnitudes
void BigInteger::sum_Magnitudes(const BigInteger& other, BigInteger& result) const
{
	result.m_Mag.resize(std::max(m_Mag.size(), other.m_Mag.size()));

	BaseType firstVal, secondVal, sumVal;
	int idx, carry = 0;

	for (idx = 0; idx < result.m_Mag.size(); ++idx)
	{
		firstVal = secondVal = 0;

		if (idx < m_Mag.size()) {
			firstVal = m_Mag[idx];
		}
		if (idx < other.m_Mag.size()) {
			secondVal = other.m_Mag[idx];
		}
		sumVal = firstVal + secondVal + carry;
		result.m_Mag[idx] = sumVal % MOD_VAL;
		carry = sumVal / MOD_VAL;
	}

	//if carry exists, add to BigInteger
	if (carry) {
		result.m_Mag.push_back(carry);
	}
}

//Get difference of BigInteger Magnitudes
void BigInteger::diff_Magnitudes(const BigInteger& other, BigInteger& result, BigInteger::COMPARE_RESULT lhs_greater) const
{
	BaseType carry = 0;
	const BigInteger *first = this, *second = &other;

	//assign first pointer to BigInteger with bigger Magnitude, second pointer to smaller
	if (lhs_greater == LESS) {
		first = &other;
		second = this;
	}

	result.m_Mag.resize(std::max(m_Mag.size(), other.m_Mag.size()));
	int index, lastNonZeroIdx = -1;
	BaseType first_val, second_val;

	for (index = 0; index < first->m_Mag.size(); ++index) {
		first_val = first->m_Mag[index] + carry;
		second_val = 0;
		//assign second value if it exists
		if (index < second->m_Mag.size()) {
			second_val = second->m_Mag[index];
		}
		//calculate result and assign appropriate carry
		if (first_val >= second_val) {
			result.m_Mag[index] = first_val - second_val;
			carry = 0;
		}
		else {
			result.m_Mag[index] = MOD_VAL + first_val - second_val;
			carry = -1;
		}

		if (result.m_Mag[index] != 0) {
			lastNonZeroIdx = index;
		}
	}
	//shrink result's magnitude to exclude leading zeros
	result.m_Mag.resize(std::max(lastNonZeroIdx + 1, 1));
}

//Multiply BigIntegers
void BigInteger::multiply(const BigInteger& other, BigInteger& result) const
{
	//if either multiplicand or multiplier is zero, return zero
	if (this->isZero() || other.isZero()) {
		result = BaseType(0);
		return;
	}

	BaseType current_value, carry = 0;
	int offset_idx = 0, result_index;

	//loop through second BigInt
	for (const BaseType& other_val : other.m_Mag) {
		result_index = offset_idx++;
		//loop through first BigInt
		for (const BaseType& this_val : this->m_Mag) {
			//multiply value at index of second to index of first, add carry from previous calculation
			current_value = (other_val * this_val) + carry;

			if (result_index < result.m_Mag.size()) { //if result fits
				current_value += result.m_Mag[result_index];
				result.m_Mag[result_index] = (current_value % MOD_VAL);
			}
			else
				result.m_Mag.push_back(current_value % MOD_VAL); //need to expand result
			carry = current_value / MOD_VAL; //compute new carry
			++result_index;
		}
		//if carry is left, expand result
		if (carry) {
			result.m_Mag.push_back(carry);
			carry = 0;
		}
	}

	result.m_sign = SIGN(m_sign * other.m_sign);
}

//Compare BigIntegers
BigInteger::COMPARE_RESULT BigInteger::compare(const BigInteger& other) const
{
	//Check signs
	if (m_sign > other.m_sign)
		return GREATER;
	if (other.m_sign > m_sign)
		return LESS;
	//check vector sizes
	if (m_Mag.size() > other.m_Mag.size())
		return GREATER;
	if (m_Mag.size() < other.m_Mag.size())
		return LESS;

	//BigIntegers have same signs and vector size. check Magnitudes
	return comp_Magnitudes(other);
}

//Compare just magnitudes of both BigIntegers
BigInteger::COMPARE_RESULT BigInteger::comp_Magnitudes(const BigInteger& other) const
{
	for (int i = m_Mag.size() - 1; i != -1; --i)
	{
		if (m_Mag[i] > other.m_Mag[i])
			return GREATER;
		if (m_Mag[i] < other.m_Mag[i])
			return LESS;
	}
	return EQUAL;
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
	return compare(other) == GREATER;
}

bool BigInteger::operator >=(const BigInteger& other) const
{
	return compare(other) != LESS;
}

//Less than
bool BigInteger::operator<(const BigInteger& other) const
{
	return compare(other) == LESS;
}

bool BigInteger::operator<=(const BigInteger& other) const
{
	return compare(other) != GREATER;
}

//Equals
bool BigInteger::operator==(const BigInteger& other) const
{
	return compare(other) == EQUAL;
}

bool BigInteger::operator!=(const BigInteger& other) const
{
	return compare(other) != EQUAL;
}

//Assignment operator
BigInteger& BigInteger::operator=(const BigInteger& other)
{
	m_Mag.clear();
	m_Mag.resize(other.m_Mag.size());
	m_Mag = other.m_Mag;
	m_sign = other.m_sign;

	return *this;
}

//Negate operator
BigInteger BigInteger::operator-(void) const
{
	BigInteger result = *this;
	result.m_sign = SIGN(result.m_sign * NEGATIVE);
	return result;
}

bool BigInteger::isNegative() const
{
	return m_sign == NEGATIVE;
}

BigInteger BigInteger::Abs() const
{
	BigInteger result = *this;
	result.m_sign = POSITIVE;
	return result;
}

//Equivalent to BigInteger == 0 to be used for special cases
bool BigInteger::isZero() const
{
	return m_Mag.size() == 1 && m_Mag.back() == 0;
}

//Addition operator
BigInteger BigInteger::operator+(const BigInteger& other) const
{
	BigInteger result;
	//Get Magnitude comparison result
	COMPARE_RESULT comp_Mag = comp_Magnitudes(other);
	//if both are of the same signs, sum their magnitudes
	if (m_sign == other.m_sign) {
		sum_Magnitudes(other, result);
		result.m_sign = m_sign;
	}
	else {
		//get Magnitude difference, assign appropriate sign
		diff_Magnitudes(other, result, comp_Mag);
		//if negative BigInteger has bigger magnitude, result's sign is negative, else positive
		//if((m_sign == NEGATIVE && comp_Mag == GREATER) || (other.m_sign == NEGATIVE && comp_Mag == LESS))

		result.m_sign = SIGN((m_sign * comp_Mag) | POSITIVE);
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
	COMPARE_RESULT comp_Mag = comp_Magnitudes(other);
	//if both BigIntegers have opposite signs, sum their magnitudes. resulting sign
	//is sign of lhs
	if (this->m_sign != other.m_sign) {
		sum_Magnitudes(other, result);
		result.m_sign = m_sign;
	}
	else {
		//get difference of magnitudes and assign appropriate sign
		diff_Magnitudes(other, result, comp_Mag);
		//if ((this->isNegative() && comp_Mag == GREATER) || (other.isNegative() && comp_Mag == LESS))
		result.m_sign = SIGN((m_sign * comp_Mag) | POSITIVE);
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
	if (bg.m_sign == bg.NEGATIVE) {
		os << '-';
	}
	os << bg.m_Mag.back();
	for_each(bg.m_Mag.rbegin() + 1, bg.m_Mag.rend(), [&bg, &os](const BigInteger::BaseType& val) {
		os << std::setfill('0') << std::setw(bg.DIGITS_PER_INDEX) << val;
	});
	return os;
}