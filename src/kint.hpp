#ifndef KINT_H
#define KINT_H

#include <queue>
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

        kint operator<<(const kint &num) const;

        bool operator>(const kint &num) const;
        bool operator<(const kint &num) const;
        bool operator==(const kint &num) const;
        bool operator!=(const kint &num) const;

    private:
        bool isNegative;
        std::vector<uint32_t> digits;

        void delFrontZero();
        size_t digitLen() const;
    };

    kint operator"" _ki(char const *value);
} // namespace kmath

#endif