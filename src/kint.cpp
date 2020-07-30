#include "kint.hpp"

using kmath::kint;

kint::kint()
{
    this->digits.push_back(0);
    this->isNegative = false;
}

kint::kint(const kint &num)
{
    this->digits = num.digits;
    this->isNegative = num.isNegative;
}

kint::kint(const int &num)
{
    this->isNegative = num < 0;
    if (this->isNegative)
        this->digits.push_back(-num);
    else
        this->digits.push_back(num);
}

kint kint::operator+(const kint &num) const
{
    if (!this->isNegative && num.isNegative)
    {
        kint temp = num;
        temp.isNegative = false;
        return *this - temp;
    }

    if (this->isNegative && !num.isNegative)
        return num + *this;

    kint result = *this;

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

    result.delFrontZero();
    return result;
}

kint kint::operator-(const kint &num) const
{
    if (!this->isNegative && num.isNegative)
    {
        kint temp = num;
        temp.isNegative = false;
        return *this + temp;
    }

    if (this->isNegative && !num.isNegative)
    {
        kint temp = *this;
        temp.isNegative = false;
        return temp + num;
    }

    if (this->isNegative && num.isNegative)
    {
        kint tempA = *this;
        kint tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB - tempA;
    }

    kint result;
    kint another;
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

    result.delFrontZero();
    return result;
}

kint kint::operator*(const kint &num) const
{
    kint result;
    kint temp;
    temp.digits.clear();

    for (auto i = num.digits.begin(); i != num.digits.end(); i++)
    {
        kint t2 = temp;
        uint32_t carry = 0;
        for (auto j = this->digits.begin(); j != this->digits.end(); j++)
        {
            uint64_t t1 = (uint64_t)*i * (uint64_t)*j + (uint64_t)carry;
            carry = t1 >> 32;
            t2.digits.push_back(t1 & __UINT32_MAX__);
        }

        if (carry != 0)
            t2.digits.push_back(carry);
        result = result + t2;
        temp.digits.push_back(0);
    }

    result.isNegative = this->isNegative ^ num.isNegative;
    result.delFrontZero();
    return result;
}

kint kint::operator/(const kint &num) const
{
    kint temp = *this;
    kint result = 0;

    while (temp > num)
    {
        kint low = 1;
        kint high = 2;
        while (high * num < temp)
        {
            low = high;
            high = high * 2;
        }
        temp = temp - low * num;
        result = result + low;
    }

    result.delFrontZero();
    return result;
}

kint kint::operator=(const kint &num)
{
    this->digits = num.digits;
    this->isNegative = num.isNegative;
    return *this;
}

bool kint::operator>(const kint &num) const
{
    if (*this == 0 && num == 0)
        return false;
    if (this->isNegative && !num.isNegative)
        return false;
    if (!this->isNegative && num.isNegative)
        return true;
    if (this->isNegative && num.isNegative)
    {
        kint tempA = *this;
        kint tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB > tempA;
    }

    if (this->digitLen() != num.digitLen())
        return this->digitLen() > num.digitLen();
    for (int i = this->digitLen() - 1; i >= 0; i--)
    {
        if (this->digits[i] == num.digits[i])
            continue;
        else
            return this->digits[i] > num.digits[i];
    }

    return false;
}

bool kint::operator<(const kint &num) const
{
    if (*this == 0 && num == 0)
        return false;
    if (this->isNegative && !num.isNegative)
        return true;
    if (!this->isNegative && num.isNegative)
        return false;
    if (this->isNegative && num.isNegative)
    {
        kint tempA = *this;
        kint tempB = num;
        tempA.isNegative = false;
        tempB.isNegative = false;
        return tempB < tempA;
    }

    if (this->digitLen() != num.digitLen())
        return this->digitLen() < num.digitLen();
    for (int i = this->digitLen() - 1; i >= 0; i--)
    {
        if (this->digits[i] == num.digits[i])
            continue;
        else
            return this->digits[i] < num.digits[i];
    }

    return false;
}

bool kint::operator==(const kint &num) const
{
    if (this->isNegative ^ num.isNegative)
        return false;
    if (this->digitLen() != num.digitLen())
        return false;

    for (int i = this->digitLen() - 1; i >= 0; i--)
    {
        if (this->digits[i] != num.digits[i])
            return false;
    }

    return true;
}

bool kint::operator!=(const kint &num) const
{
    return !(*this == num);
}

/*********************PRIVATE************************/
void kint::delFrontZero()
{
    while (this->digitLen() > 1 && this->digits.back() == 0)
        this->digits.pop_back();
}

size_t kint::digitLen() const
{
    return this->digits.size();
}
/****************************************************/

kint kmath::operator"" _ki(char const *value)
{
    size_t i = 0;
    bool isNegative = false;
    kint result;

    if (value[i] == '-')
    {
        isNegative = true;
        i++;
    }

    for (; value[i] != '\0'; i++)
    {
        if (value[i] < '0' || value[i] > '9')
            break;
        result = result * 10;
        result = result + (value[i] - '0');
    }

    if (isNegative && result != 0)
        result = result * -1;

    return result;
}