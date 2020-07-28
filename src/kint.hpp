#ifndef KINT_H
#define KINT_H

#include <vector>
#include <cstdint>

namespace kmath
{
    class kint
    {
    public:
        kint();
        kint(const kint &num);
        kint(const int &num);

        kint operator+(const kint &num) const;
        kint operator-(const kint &num) const;
        kint operator*(const kint &num) const;
        kint operator/(const kint &num) const;
        kint operator=(const kint &num);

        bool operator>(const kint &num) const;
        bool operator<(const kint &num) const;
        bool operator==(const kint &num) const;

        size_t digitLen() const;

    private:
        bool isNegative;
        std::vector<uint32_t> digits;
    };

    kint operator"" _ki(char const *value, size_t len);
} // namespace kmath

#endif