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
        result.digits[i] += num.digits[i];
        uint32_t temp = result.digits[i] > __UINT32_MAX__ - num.digits[i] ? 1 : 0;
        result.digits[i] += carry;
        if (temp == 0)
            temp = result.digits[i] > __UINT32_MAX__ - carry ? 1 : 0;
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

    KInt result = *this;
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
    for (int i = 0; i < this->digitLen(); i++)
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
    for (int i = 0; i < this->digitLen(); i++)
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