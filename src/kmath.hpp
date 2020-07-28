#ifndef KMATH_H
#define KMATH_H

#include <vector>
#include <cstdint>

class KInt
{
public:
    KInt();
    KInt(const KInt &num);

    KInt operator+(const KInt &num) const;
    KInt operator-(const KInt &num) const;
    KInt operator*(const KInt &num) const;
    KInt operator/(const KInt &num) const;
    KInt operator=(const KInt &num);

    bool operator>(const KInt &num) const;
    bool operator<(const KInt &num) const;
    bool operator==(const KInt &num) const;

    size_t digitLen() const;

private:
    bool isNegative;
    std::vector<uint32_t> digits;
};

KInt operator""k(char const *value, size_t len);

#endif