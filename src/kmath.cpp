#include "kmath.hpp"

KInt::KInt()
{
    this->digits.push_back(0);
    this->isNegative = false;
}

KInt::KInt(const KInt &num)
{
    *this = num;
}

KInt KInt::operator+(const KInt &num) const
{
    if (!this->isNegative && num.isNegative)
    {
        KInt temp = num;
        temp.isNegative = false;
        return *this - temp;
    }

    if (this->isNegative && !num.isNegative)
        return num + *this;

    KInt result = *this;

    size_t minLen = result.digitLen();
    if (minLen > num.digitLen())
        minLen = num.digitLen();

    uint32_t carry = 0;
    for (size_t i = 0; i < minLen; i++)
    {
        uint32_t temp = result.digits[i] > __UINT32_MAX__ - num.digits[i] ? 1 : 0;
        result.digits[i] += num.digits[i];
        if (temp == 0)
            temp = result.digits[i] > __UINT32_MAX__ - carry ? 1 : 0;
        result.digits[i] += carry;
        carry = temp;
    }

    if (minLen == this->digitLen())
    {
        for (size_t i = minLen; i < num.digitLen(); i++)
            result.digits.push_back(num.digits[i]);
    }

    if (minLen == result.digitLen())
        result.digits.push_back(carry);
    else
        result.digits[minLen] += carry;

    return result;
}

KInt KInt::operator-(const KInt &num) const
{
    if (!this->isNegative && num.isNegative)
    {
        KInt temp = num;
        temp.isNegative = false;
        return *this + temp;
    }

    if (this->isNegative && !num.isNegative)
    {
        KInt temp = *this;
        temp.isNegative = false;
        return temp + num;
    }

    if (this->isNegative && num.isNegative)
    {
        KInt tempA = *this;
        KInt tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB - tempA;
    }

    KInt result;
    KInt another;
    if (*this < num)
    {
        result = num;
        another = *this;
        result.isNegative = true;
    }
    else
    {
        result = *this;
        another = num;
    }

    size_t minLen = another.digitLen();
    uint32_t carry = 0;
    for (size_t i = 0; i < minLen; i++)
    {
        uint32_t temp = result.digits[i] < another.digits[i] ? 1 : 0;
        result.digits[i] -= another.digits[i];
        if (temp == 0)
            temp = result.digits[i] < carry ? 1 : 0;
        result.digits[i] -= carry;
        carry = temp;
    }

    if (result.digitLen() > minLen)
        result.digits[minLen] -= carry;
    
    while (result.digitLen() > 1 && result.digits.back() == 0)
        result.digits.pop_back();
    if (result.digitLen() == 1 && result.digits[0] == 0)
        result.isNegative = false;

    return result;
}

KInt KInt::operator=(const KInt &num)
{
    this->digits = num.digits;
    this->isNegative = num.isNegative;
    return *this;
}

bool KInt::operator>(const KInt &num) const
{
    if (this->isNegative && !num.isNegative)
        return false;
    if (!this->isNegative && num.isNegative)
        return true;
    if (this->isNegative && num.isNegative)
    {
        KInt tempA = *this;
        KInt tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB > tempA;
    }

    if (this->digitLen() != num.digitLen())
        return this->digitLen() > num.digitLen();
    for (size_t i = this->digitLen() - 1; i >= 0; i--)
    {
        if (this->digits[i] > num.digits[i])
            return true;
    }

    return false;
}

bool KInt::operator<(const KInt &num) const
{
    if (this->isNegative && !num.isNegative)
        return true;
    if (!this->isNegative && num.isNegative)
        return false;
    if (this->isNegative && num.isNegative)
    {
        KInt tempA = *this;
        KInt tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB < tempA;
    }

    if (this->digitLen() != num.digitLen())
        return this->digitLen() < num.digitLen();
    for (size_t i = this->digitLen() - 1; i >= 0; i--)
    {
        if (this->digits[i] < num.digits[i])
            return true;
    }

    return false;
}

bool KInt::operator==(const KInt &num) const
{
    return !(*this > num || *this < num);
}

size_t KInt::digitLen() const
{
    return this->digits.size();
}