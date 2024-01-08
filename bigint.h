/****************************************************************************************/
/*                                                                                      */
/*    bigint.h                                                                          */
/*                                                                                      */
/*    bigint Library for C++                                                            */
/*                                                                                      */
/*    MIT License                                                                       */
/*    Copyright (c) 2023-2024 François-Xavier David                                     */
/*                                                                                      */
/*    Permission is hereby granted, free of charge, to any person obtaining a copy      */
/*    of this software and associated documentation files (the "Software"), to deal     */
/*    in the Software without restriction, including without limitation the rights      */
/*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         */
/*    copies of the Software, and to permit persons to whom the Software is             */
/*    furnished to do so, subject to the following conditions:                          */
/*                                                                                      */
/*    The above copyright notice and this permission notice shall be included in all    */
/*    copies or substantial portions of the Software.                                   */
/*                                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        */
/*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          */
/*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       */
/*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            */
/*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     */
/*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/*    SOFTWARE.                                                                         */
/*                                                                                      */
/****************************************************************************************/

#pragma once
#ifndef BIGINT_H
#define BIGINT_H

#include <algorithm>
#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#if __has_include(<intrin.h>)
#include <intrin.h>
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define _bigint_x86_x64_ARCH
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define _bigint_x86_x64_ARCH
#endif

#define NOMINMAX
#include <Windows.h>

using limb  = uint32_t;
using llimb = uint64_t;
using limbs = std::basic_string<limb>;


#pragma warning(push)
#pragma warning(disable: 4710 4711)

namespace bigint {
    // Big Integer Class
    class bigint_t {
    public:
        limbs str;
#pragma warning( disable : 4820 )
        signed char sign;

        static void trim(limbs&);

        static int compare(limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);

        static int compare(const limbs& str1, const limbs& str2);

        static void add(limbs::iterator,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void addInto(limbs::iterator       into,
            limbs::const_iterator str_begin, limbs::const_iterator str_end);
        static void subtract(limbs::iterator,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void subtractFrom(limbs::iterator       from,
            limbs::const_iterator str_begin, limbs::const_iterator str_end);
        static void _multiply(limbs&, const limbs&, const limbs&);
        static void shiftLeft(limbs&, size_t);
        static void shiftRight(limbs&, size_t);


        static limbs multiply(const limbs& str1, const limbs& str2);
        static void mult(limbs::iterator       product_begin, limbs::iterator       product_end,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void mult_vanilla(limbs::iterator       product_begin, limbs::iterator       product_end,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void mult_karatsuba(limbs::iterator       product_begin, limbs::iterator       product_end,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void mult_karatsuba_bugged(limbs::iterator product_begin, limbs::iterator       product_end,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
        static void mult_toom3(limbs::iterator       product_begin, limbs::iterator       product_end,
            limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
            limbs::const_iterator str2_begin, limbs::const_iterator str2_end);

        //static void multiply_schonhageStrassen(limbs&, const limbs& str1, const limbs& str2);
        static void divide(limbs& quotient, limbs& remainder, const limbs& str1, const limbs& str2);
        static void div(limbs::iterator       quotient, limbs& remainder,
            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
            limbs::const_iterator divisor_begin, limbs::const_iterator divisor_end);
        static void div_vanilla(limbs::iterator       quotient, limbs& remainder,
            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
            limbs::const_iterator divisor_begin, limbs::const_iterator divisor_end);

        static void  mod(limbs&, limbs&, limbs, limbs);

        static bool  is_bigint(const std::string&);

        static void  swap(limbs&, limbs&);
        static limbs sqrt(limbs);
        static limbs gcd(limbs, limbs);
        static limbs lcm(limbs, limbs);
        static bool  isPrime(limbs);

        static void fromString(limbs& destination, signed char& sign, const std::string& source)
        {
            destination.push_back(0);
            if (source.size() == 0) {
                sign = 1;
                return;
            }
            if (source[0] == '-')
                sign = -1;
            else
                sign = 1;

            auto d = source.begin();
            if (sign < 0)
                std::advance(d, 1);
            for (; d != source.end(); ++d) {
                if (*d < '0' || *d > '9')
                    throw std::runtime_error("Invalid Big Integer has been fed.");

                llimb carry = static_cast<llimb>(*d) - '0';
                for (auto i = destination.begin(); i != destination.end(); std::advance(i, 1)) {
                    llimb value = static_cast<llimb>(*i) * 10 + carry;
                    *i = static_cast<limb>(value);
                    carry = value >> 32;
                }
                if (carry)
                    destination.push_back(static_cast<limb>(carry));
            }
        }


    public:
        bigint_t() :
            str(1, 0),
            sign(1)
        {
        }
        bigint_t(const std::string& source) :
            str(),
            sign(source[0] == '-' ? -1 : 1)
        {
            fromString(str, sign, source);
        }
        bigint_t(long long int n) :
            str(),
            sign(n >= 0 ? 1 : -1)
        {
            n = sign * n;
            str.push_back(static_cast<limb>(n));
            if (n > (1LL << 32))
                str.push_back(static_cast<limb>(n >> 32));
        }
        bigint_t(unsigned long long int n) :
            str(),
            sign(1)
        {
            n = sign * n;
            str.push_back(static_cast<limb>(n));
            if (n > (1LL << 32))
                str.push_back(static_cast<limb>(n >> 32));
        }

        bigint_t(const bigint_t& n) = default;

        void output(std::ostream& stream, unsigned short base = 10) const;
        void output_fast(std::ostream& stream, unsigned short base) const;
        std::string toString(unsigned short base = 10) const;

        //Work in progress: speed up the output methods
        //void output_q(std::ostream& stream, unsigned short base = 10) const;
        //std::string toString_q(unsigned short base = 10) const;

        friend std::ostream& operator << (std::ostream& stream, const bigint_t& n) {
            switch (stream.flags() & std::ios_base::basefield) {
            case stream.oct: n.output_fast(stream, 8); return stream;
            case stream.hex: n.output_fast(stream, 16); return stream;
            case stream.dec:
            default:         n.output(stream, 10); return stream;
            }
        }
        friend std::istream& operator >> (std::istream& stream, bigint_t& n) {
            std::string string_val;
            stream >> string_val;
            fromString(n.str, n.sign, string_val);
            return stream;
        }

        bigint_t operator + (bigint_t const& n) const {
            bigint_t ans;
            if (sign == n.sign) {
                ans.str.resize(std::max(str.size(), n.str.size()) + 1, 0);
                add(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
            }
            else {
                auto comparison = compare(str, n.str);
                switch (comparison)
                {
                case 0: {
                    ans.sign = 1;
                    ans.str = { 0 };
                    return ans;
                }
                case -1: {
                    ans.str.resize(n.str.size(), 0);
                    subtract(ans.str.begin(), n.str.cbegin(), n.str.cend(), str.cbegin(), str.cend());
                    ans.sign = -sign;
                    break;
                }
                case 1: {
                    ans.str.resize(str.size(), 0);
                    subtract(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
                    ans.sign = sign;
                    break;
                }
                }
            }
            trim(ans.str);
            return ans;
        }
        friend bigint_t operator + (bigint_t const& n1, long long int n2) {
            return n1 + bigint_t(n2);
        }
        friend bigint_t operator + (long long int n1, bigint_t const& n2) {
            return bigint_t(n1) + n2;
        }
        bigint_t& operator += (bigint_t const& n) {
            if (sign == n.sign) {
                if (n.str.size() >= str.size())
                    str.resize(n.str.size() + 1, 0);
                else if (str[str.size() - 1] == static_cast<limb>((1ULL << 32) - 1))
                    str.resize(str.size() + 1, 0);
                addInto(str.begin(), n.str.cbegin(), n.str.end());
            }
            else {
                auto comparison = compare(str, n.str);
                switch (comparison)
                {
                case 0: {
                    sign = 1;
                    str = { 0 };
                    break;
                }
                case -1: {
                    str.resize(n.str.size(), 0);
                    subtract(str.begin(), n.str.cbegin(), n.str.cend(), str.cbegin(), str.end());
                    sign = -sign;
                    break;
                }
                case 1: {
                    subtractFrom(str.begin(), n.str.cbegin(), n.str.end());
                    break;
                }
                }
            }
            trim(str);
            return *this;
        }

        bigint_t operator - (bigint_t const& n) const
        {
            bigint_t ans;
            if (sign != n.sign) {
                ans.str.resize(std::max(str.size(), n.str.size()) + 1, 0);
                ans.sign = sign;
                add(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
            }
            else {
                auto comparison = compare(str, n.str);
                switch (comparison)
                {
                case 0: {
                    ans.sign = 1;
                    ans.str = { 0 };
                    return ans;
                }
                case -1: {
                    ans.str.resize(n.str.size(), 0);
                    subtract(ans.str.begin(), n.str.cbegin(), n.str.cend(), str.cbegin(), str.cend());
                    ans.sign = -sign;
                    break;
                }
                case 1: {
                    ans.str.resize(str.size(), 0);
                    subtract(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
                    ans.sign = sign;
                    break;
                }
                }
            }
            trim(ans.str);
            return ans;
        }
        friend bigint_t operator - (bigint_t const& n1, long long int n2)
        {
            return n1 - bigint_t(n2);
        }
        friend bigint_t operator - (long long int n1, bigint_t const& n2)
        {
            return bigint_t(n1) - n2;
        }
        bigint_t& operator -= (bigint_t const n) {
            if (sign != n.sign) {
                str.resize(std::max(str.size(), n.str.size()) + 1, 0);
                addInto(str.begin(), n.str.cbegin(), n.str.end());
            }
            else {
                auto comparison = compare(str, n.str);
                switch (comparison)
                {
                case 0: {
                    sign = 1;
                    str = { 0 };
                    break;
                }
                case -1: {
                    str.resize(n.str.size(), 0);
                    subtract(str.begin(), n.str.cbegin(), n.str.cend(), str.cbegin(), str.end());
                    sign = -sign;
                    break;
                }
                case 1: {
                    subtractFrom(str.begin(), n.str.cbegin(), n.str.end());
                    break;
                }
                }
            }
            trim(str);
            return *this;
        }

        bigint_t operator * (bigint_t const& n) const {
            bigint_t ans;
            ans.sign = sign * n.sign;
            ans.str = multiply(str, n.str);
            return ans;
        }
        friend bigint_t operator * (bigint_t const& n1, long long int n2)
        {
            return n1 * bigint_t(n2);
        }
        friend bigint_t operator * (long long int n1, bigint_t const& n2)
        {
            return bigint_t(n1) * n2;
        }

        bigint_t& operator *= (bigint_t const n) {
            sign *= n.sign;
            str = multiply(str, n.str);
            return *this;
        }

        bigint_t operator / (bigint_t const& n) const
        {
            limbs quotient, remainder;
            divide(quotient, remainder, str, n.str);
            bigint_t ans; ans.str = quotient;
            return ans;
        }
        friend bigint_t operator / (bigint_t const& n1, long long n2)
        {
            return n1 / bigint_t(n2);
        }
        friend bigint_t operator / (long long n1, bigint_t const& n2)
        {
            if (n2.str.size() > 2)
                return bigint_t(0LL);
            return bigint_t(n1) / n2;
        }
        bigint_t& operator /= (bigint_t const n)
        {
            limbs quotient, remainder;
            divide(quotient, remainder, str, n.str);
            str.swap(quotient);
            return *this;
        }


        bigint_t operator % (bigint_t const& n) const {
            bigint_t ans;
            limbs quotient, remainder;
            mod(quotient, remainder, str, n.str);
            ans.str = remainder;
            return ans;
        }
        friend bigint_t operator % (bigint_t const& n1, long long n2) {
            return n1 % bigint_t(n2);
        }
        friend bigint_t operator % (long long n1, bigint_t const& n2) {
            if (n2.str.size() > 2)
                return n1;
            return bigint_t(n1) % n2;
        }
        bigint_t& operator %= (bigint_t const n) {
            limbs quotient, remainder;
            mod(quotient, remainder, str, n.str);
            std::swap(str, remainder);
            return *this;
        }

        bigint_t operator<<(size_t value)
        {
            bigint_t ans(*this);
            shiftLeft(ans.str, value);
            return ans;
        }
        bigint_t operator>>(size_t value)
        {
            bigint_t ans(*this);
            shiftRight(ans.str, value);
            return ans;
        }
        bigint_t& operator<<=(size_t value)
        {
            shiftLeft(str, value);
            return *this;
        }
        bigint_t& operator>>=(size_t value)
        {
            shiftRight(str, value);
            return *this;
        }

        bigint_t& operator ++ () {
            if (sign >= 0) {
                for (auto& s : str) {
                    if (s < (1ULL << 32) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                str.push_back(1);
            }
            else if (str.size() == 1 && str[0] == 1) {
                str[0] = 0;
                sign = 1;
            }
            else {
                for (auto& s : str) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << 32) - 1);
                }
                trim(str);
            }
            return *this;
        }
        bigint_t  operator ++ (int v) {
            if (v <= 0)
                v = 1;
            if (sign >= 0) {
                if (str[0] < (1ULL << 32) - v) {
                    str[0] += v;
                    return *this;
                }
                else if (v > 1) {
                    v -= static_cast<int>((1ULL << 32) - str[0]);
                    str[0] = static_cast<limb>((1ULL << 32) - 1);
                }
                for (auto& s : str) {
                    if (s < (1ULL << 32) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                str[0] += v;
                str.push_back(1);
            }
            else if (str.size() == 1 && str[0] == 1) {
                str[0] = 0;
                sign = 1;
            }
            else {
                for (auto& s : str) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << 32) - 1);
                }
                trim(str);
            }
            return *this;
        }
        bigint_t& operator -- () {
            if (sign < 0) {
                for (auto& s : str) {
                    if (s < (1ULL << 32) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                str.push_back(1);
            }
            else if (str.size() == 1 && str[0] == 0) {
                str[0] = 1;
                sign = -1;
            }
            else {
                for (auto& s : str) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << 32) - 1);
                }
                trim(str);
            }
            return *this;
            return *this;
        }
        bigint_t  operator -- (int v) {
            if (v <= 0)
                v = 1;
            if (sign < 0) {
                if (str[0] < (1ULL << 32) - v) {
                    str[0] += v;
                    return *this;
                }
                else if (v > 1) {
                    v -= static_cast<int>((1ULL << 32) - str[0]);
                    str[0] = static_cast<limb>((1ULL << 32) - 1);
                }
                for (auto& s : str) {
                    if (s < (1ULL << 32) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                str[0] += v;
                str.push_back(1);
            }
            else if (str.size() == 1 && str[0] == 0) {
                str[0] = 1;
                sign = -1;
            }
            else {
                for (auto& s : str) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << 32) - 1);
                }
                trim(str);
            }
            return *this;
        }

        bool operator ==(bigint_t const& n) const {
            return sign == n.sign && str == n.str;
        }
        bool operator !=(bigint_t const& n) {
            return sign != n.sign || str != n.str;
        }
        bool operator > (bigint_t const& n) const {
            if (sign > n.sign)
                return true;
            else if (sign > 0)
                return compare(str, n.str) > 0;
            else
                return compare(str, n.str) < 0;
        }
        bool operator < (bigint_t const& n) const {
            if (sign < n.sign)
                return true;
            else if (sign > 0)
                return compare(str, n.str) < 0;
            else
                return compare(str, n.str) > 0;

        }
        bool operator >=(bigint_t const& n) const {
            if (sign > n.sign)
                return true;
            else if (sign > 0)
                return compare(str, n.str) >= 0;
            else
                return compare(str, n.str) <= 0;
        }
        bool operator <=(bigint_t const& n) const {
            if (sign < n.sign)
                return true;
            else if (sign > 0)
                return compare(str, n.str) <= 0;
            else
                return compare(str, n.str) >= 0;
        }
 
#if __cplusplus >= 202002L
        std::strong_ordering operator<=>(bigint_t const& n)
        {
            if (sign != n.sign)
                return sign <=> n.sign;
            else {
                if (auto comparison = compare(str, n.str); comparison == 0)
                    return std::strong_ordering::equal;
                else if (sign == comparison)
                    return std::strong_ordering::greater;
                else
                    return std::strong_ordering::less;
            }
        }
#endif

        static bool _big_isPrime(bigint_t& a) {
            return isPrime(a.str);
        }

        friend bigint_t power(const bigint_t&, size_t);
        friend bigint_t factorial(size_t n);

    };

    bigint_t power(const bigint_t& n, size_t p)
    {
        bigint_t result = 1ULL, cache;
        result.sign = (p & 1) ? n.sign : 1; cache.str = n.str;

        while (p > 0) {
            if (p & 1)
                result *= cache;
            cache *= cache;
            p >>= 1;
        }
        return result;
    }
    bigint_t factorial(size_t input)
    {
        if (input <= 20) {
            llimb result = 1;
            for (limb i = 2; i <= input; ++i)
                result *= i;
            return bigint_t(result);
        }

        limbs result{ 1 },
            cache{ 1 };
        size_t progress = 3,
            iterator = 3,
            half = input >> 1;

        size_t powOf2 = 0,
            totalPowOf2 = 0;

        llimb last = input;
        while (last >= 6) {
            last = last >> 1;
            powOf2 += 1;
        }
        progress = last >= 5ULL ? 5ULL : 3ULL;
        totalPowOf2 = (powOf2 + 2) * (powOf2 + (last >= 4 ? 3 : 1)) / 2;

        while (progress <= half) {
#pragma warning(suppress: 4293)
            limbs newCache = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 32) };
            totalPowOf2 += powOf2 * (powOf2 + 1) / 2;
            iterator += 2;
            for (; iterator <= progress; iterator += 2) {
#pragma warning(suppress: 4293)
                limbs val = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 32) };
                newCache = bigint_t::multiply(newCache, val);
                totalPowOf2 += powOf2 * (powOf2 + 1) / 2;
            }
            cache  = bigint_t::multiply(cache, newCache);
            result = bigint_t::multiply(result, cache);

            if (powOf2 <= 1)
                break;
            progress = progress << (powOf2 - 2);
            while (progress < input)
                progress += 1;
            progress = progress >> (powOf2 - 1);
            powOf2 -= 1;
        }

#pragma warning(suppress: 4293)
        limbs newCache = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 32) };
        iterator += 2;
        for (; iterator <= input; iterator += 2)
#pragma warning(suppress: 4293)
            newCache = bigint_t::multiply(newCache, { static_cast<limb>(iterator), static_cast<limb>(iterator >> 32) });

        cache  = bigint_t::multiply(cache, newCache);
        result = bigint_t::multiply(result, cache);

        bigint_t::shiftLeft(result, totalPowOf2);

        bigint_t ans; ans.str = result;
        return ans;
    }

#if __cplusplus >= 202002L
    template<unsigned int value, unsigned int lowerBound>
    concept lower_bound_value = value >= lowerBound;
#endif

#pragma region Fibonacci functions
    namespace {
        template<typename T, unsigned int order>
        class FiboMatrix {
            static_assert(order >= 2);
        public:
            FiboMatrix()
                : vals{ bigint_t(0LL) }
            {
                for (size_t i = 0; i < order; ++i)
                    vals[i] = bigint_t(1LL);
                for (size_t r = 2; r < order; ++r)
                    vals[r * order - 1] = bigint_t(1LL);
            }
            FiboMatrix(std::array<T, order> values)
                : vals{ bigint_t(0LL) }
            {
                for (auto r = 0; r < order; ++r)
                    vals[r * (order - 1)] = values[order - r - 1];

                T val = values[order - 1];
                for (auto i = 0; i < order - 1; ++i)
                    val -= values[i];
                vals[order * order - order] = val;

                for (size_t c = order - 2; c >= 1; --c) {
                    for (size_t r = 0; r <= c; ++r)
                        vals[r * (order - 1) + c] = vals[r * (order - 1) + order - 1] + vals[(r + 1) * (order - 1) + c + 1];
                }
                for (size_t c = 1; c < order - 1; ++c) {
                    for (size_t r = c + 1; r < order; ++r)
                        vals[r * (order - 1) + c] = vals[(r - 1) * (order - 1) + c - 1] - vals[(r - 1) * (order - 1) + order - 1];
                }
            }
            T* operator[](size_t r) const {
                return const_cast<T*>(&vals[r * (order - 1)]);
            }

            FiboMatrix<T, order> operator * (FiboMatrix<T, order> const& other) const {
                FiboMatrix<T, order> result;
                for (size_t c = 0; c < order; ++c) {
                    result[order - 1][c] = bigint_t(0LL);
                    for (size_t i = 0; i < order; ++i)
                        result[order - 1][c] += ((*this)[order - 1][i]) * other[i][c];
                }
                for (size_t r = order - 1; r >= 1; --r) {
                    for (size_t c = order - 1; c >= 1; --c) {
                        result[r - 1][c - 1] = result[r - 1][order - 1] + result[r][c];
                    }
                }
                return result;
            }

        private:
            std::array<T, order* order - order + 1> vals;
        };


        template <unsigned int order>
        void matrix_power(FiboMatrix<bigint_t, order>& result, FiboMatrix<bigint_t, order>& M, unsigned int exp) {
            while (exp > 1) {
                if ((exp & 1) == 1)
                    result = M * result;
                M = M * M;
                exp = exp >> 1;
            }
        }


        template <unsigned int order>
        void fibonacci_matrix(std::vector<bigint_t>& result, unsigned int n, FiboMatrix<bigint_t, order>& initial_value_matrix) {
            if (n <= order) {
                for (unsigned int i = 1; i < order; ++i)
                    result.push_back(bigint_t(0LL));
                result.push_back(bigint_t(1LL));
                return;
            }
            FiboMatrix<bigint_t, order> fibo_matrix;
            //Matrix exponantiation in 2 steps: first = actual matrix multiplication. It falls 1 step short.
            matrix_power<order>(initial_value_matrix, fibo_matrix, n);
            // The last matrix multiplication is shortened to only the Fibonacci values (i.e. NOT the middle of the matrix).
            bigint_t nextFibo;
            for (unsigned int r = 1; r <= order; ++r) {
                bigint_t curFibo;
                for (unsigned int i = 0; i < order; ++i) {
                    curFibo += initial_value_matrix[order - r][i] * fibo_matrix[i][order - 1];
                }
                result.push_back(curFibo);
                nextFibo += curFibo;
            }
            result.push_back(nextFibo);
        }

        template <unsigned int order = 2, typename std::enable_if <order <= 3> ::type* = nullptr>
        void fibonacci_consecutive(std::vector<bigint_t>& result, unsigned int count)
        {
            auto curValue = result.cbegin();
            std::advance(curValue, 1);
            for (auto i = 0; i < count; ++i) {
                bigint_t newValue(*curValue);
                std::advance(curValue, 1);
                for (auto j = curValue; j != result.cend(); ++j)
                    newValue += *j;
                result.push_back(newValue);
            }
        }
        template <unsigned int order = 2, typename std::enable_if <order >= 4> ::type* = nullptr>
        void fibonacci_consecutive(std::vector<bigint_t>& result, unsigned int count)
        {
            auto prevValue = result.cbegin(), curValue = result.cend();
            std::advance(curValue, -1);
            for (auto i = 0; i < count; ++i) {
                bigint_t newValue(*curValue);
                bigint_t::shiftLeft(newValue.str, 1);
                newValue -= *prevValue;
                result.push_back(newValue);
                std::advance(prevValue, 1);
                std::advance(curValue, 1);
            }
        }
    };



    template <unsigned int order>
#if __cplusplus >= 202002L
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to, std::array<bigint_t, order> firstValues) requires lower_bound_value<order, 2>
#else
    void fibonacci<order true>(std::vector<bigint_t>& result, unsigned int from, unsigned int to, std::array<bigint_t, order> firstValues)
#endif
    {
        result.reserve(to - from + 1);
        size_t element_size = 1;
        if (from > order * order) {
            auto greatest = std::max_element(firstValues.cbegin(), firstValues.cend(), [](auto a, auto b) { return a.str.size() < b.str.size(); });
            element_size = greatest->str.size();
        }
        if (from <= order * order * element_size) { // Special case: from is too small for calls to fibonacci_matrix to make sense
            std::vector<bigint_t> intermediate;
            intermediate.reserve(from + order + 1);
            bigint_t nextFibo;
            for (auto v : firstValues) {
                intermediate.push_back(v);
                nextFibo += v;
            }
            intermediate.push_back(nextFibo);
            fibonacci_consecutive<order>(intermediate, from - 1);
            auto i = intermediate.cend();
            i -= (order + 1);
            for (; i != intermediate.cend(); ++i)
                result.push_back(*i);
        }
        else { // General case: from is big enough to use the matrix exponantiation algorithm
            FiboMatrix<bigint_t, order> initial_value_matrix(firstValues);
            fibonacci_matrix<order>(result, from, initial_value_matrix);
        }
        fibonacci_consecutive<order>(result, to - from - order);
    }
    template <unsigned int order = 2>
#if __cplusplus >= 202002L
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to) requires lower_bound_value<order, 2>
#else
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to)
#endif
    {
        std::array<bigint_t, order> standardFirstValues{ bigint_t(0LL) };
        standardFirstValues[order - 1] = standardFirstValues[order - 2] = bigint_t(1LL);
        return fibonacci(result, from, to, standardFirstValues);
    }

#pragma endregion

};

template<class T>
class Reverse
{
    T const& rng;
public:
    Reverse(T const& r) noexcept
        :
        rng(r)
    {}
    Reverse(Reverse<T>&) = delete;
    Reverse(Reverse<T>&&) = delete;


    auto begin() const noexcept { using std::end;   return std::make_reverse_iterator(end(rng)); }
    auto end()   const noexcept { using std::begin; return std::make_reverse_iterator(begin(rng)); }
};


bool bigint::bigint_t::is_bigint(const std::string& s) {
    size_t start = 0;
    if (s[0] == '-')
        start = 1;
    for(size_t i = start; i < s.size(); ++i) {
        if(!((int)s[i] > 47 && (int)s[i] < 58))
            return false;
    }
    return true;
}

#include <iostream>


void bigint::bigint_t::output(std::ostream& stream, unsigned short base) const
{
    if (base < 2)
        throw std::runtime_error("Base is invalid");
    if (base > 64)
        throw std::runtime_error("Base is not supported");
    if (base == 2 || base == 4 || base == 8) {
        output_fast(stream, base);
        return;
    }
    if (sign < 0)
        stream << '-';

    auto print = [](auto name, const limbs& value) {
        OutputDebugString(name);
        OutputDebugString(L": ");
        for (auto i = value.crbegin(); i != value.crend(); ++i)
            OutputDebugString((std::to_wstring(*i) + L" ").c_str());
        OutputDebugString(L"\n");
    };

    limb basePower[63] = {
                 0, 3486784401,          0, 1220703125, 2176782336, 1977326743, 1073741824, 3486784401,
        1000000000, 2357947691,  429981696,  815730721, 1475789056, 2562890625,          0,  410338673,
         612220032,  893871739, 1280000000, 1801088541, 2494357888, 3404825447,  191102976,  244140625,
         308915776,  387420489,  481890304,  594823321,  729000000,  887503681, 1073741824, 1291467969,
        1544804416, 1838265625, 2176782336, 2565726409, 3010936384, 3518743761, 4096000000,  115856201,
         130691232,  147008443,  164916224,  184528125,  205962976,  229345007,  254803968,  282475249,
         312500000,  345025251,  380204032,  418195493,  459165024,  503284375,  550731776,  601692057,
         656356768,  714924299,  777600000,  844596301,  916132832,  992436543, 1073741824
    };
    size_t expLimbLen[63] = { 32, 20, 16, 13, 12, 11, 10, 10, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

    limb big_base = basePower[base - 2];
    limbs result = { 0 };
    std::string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    for (auto d : Reverse<limbs>(str)) {
        llimb carry = 0;
        for (auto& s : result) {
            llimb value = (static_cast<llimb>(s) << 32) + carry;
            s = value % big_base;
            carry = value / big_base;
        }
        if (carry > big_base) {
            result.push_back(static_cast<limb>(carry % big_base));
            result.push_back(static_cast<limb>(carry / big_base));
        }
        else if (carry)
            result.push_back(static_cast<limb>(carry));

        carry = d;
        if (carry) {
            for (auto& r : result) {
                llimb value = r + carry;
                r = value % big_base;
                carry = value / big_base;
                if (!carry)
                    break;
            }
            if (carry)
                result.push_back(static_cast<limb>(carry));
        }
    }
    auto r = result.rbegin();
    stream << *r;
    std::advance(r, 1);

    for (; r != result.rend(); ++r) {
        std::string value = std::to_string(*r);
        stream << std::string(expLimbLen[base - 2] - value.size(), '0');
        stream << *r;
    }
}

std::string bigint::bigint_t::toString(unsigned short base) const
{
    std::stringstream outputStream;
    output(outputStream, base);
    return outputStream.str();
}



void bigint::bigint_t::output_fast(std::ostream& stream, unsigned short base) const
{
}

//void bigint::bigint_t::output_q(std::ostream& stream, unsigned short base) const
//{
//    if (base < 2)
//        throw std::runtime_error("Base is invalid");
//    if (base > 64)
//        throw std::runtime_error("Base is not supported");
//    if (base == 2 || base == 4 || base == 8) {
//        output_fast(stream, base);
//        return;
//    }
//    if (sign < 0)
//        stream << '-';
//
//    limb basePower[63] = {
//                 0, 3486784401,          0, 1220703125, 2176782336, 1977326743, 1073741824, 3486784401,
//        1000000000, 2357947691,  429981696,  815730721, 1475789056, 2562890625,          0,  410338673,
//         612220032,  893871739, 1280000000, 1801088541, 2494357888, 3404825447,  191102976,  244140625,
//         308915776,  387420489,  481890304,  594823321,  729000000,  887503681, 1073741824, 1291467969,
//        1544804416, 1838265625, 2176782336, 2565726409, 3010936384, 3518743761, 4096000000,  115856201,
//         130691232,  147008443,  164916224,  184528125,  205962976,  229345007,  254803968,  282475249,
//         312500000,  345025251,  380204032,  418195493,  459165024,  503284375,  550731776,  601692057,
//         656356768,  714924299,  777600000,  844596301,  916132832,  992436543, 1073741824
//    };
//    size_t expLimbLen[63] = { 32, 20, 16, 13, 12, 11, 10, 10, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
//
//    constexpr size_t size_threshold = 32768;
//    size_t str_len = str.size();
//    if (str_len >= size_threshold) {
//        std::cerr << "Begin toString:\n";
//        bigint_t big_base{ static_cast<llimb>(basePower[base - 2]) };
//        size_t half_len = str_len / 2 + 1;
//        //do {
//        //    big_base *= big_base;
//        //    half_len = half_len >> 1;
//        //} while (big_base.str.size() < size_threshold);
//        do {
//            big_base *= big_base;
//            half_len = half_len >> 1;
//        } while (half_len > 1);
//        limbs q, r;
//        bigint::bigint_t::divide(q, r, str, big_base.str);
//        trim(q); trim(r);
//        bigint_t quotient, remainder;
//        quotient.str = q; remainder.str = r;
//        //std::cerr << toString_slow() << '\n';
//        //std::cerr << big_base.toString_slow() << '\n';
//        //std::cerr << quotient.toString_slow() << '\n';
//        //std::cerr << remainder.toString_slow() << '\n';
//        quotient.output(stream, base);
//        auto sr = remainder.toString(base);
//        if (r.size() * expLimbLen[base - 2] > sr.size())
//            stream << std::string(r.size() * expLimbLen[base - 2] - sr.size(), 0) << sr;
//        //std::cerr << "End toString" << std::endl;
//    }
//
//    limb big_base = basePower[base - 2];
//    limbs result = { 0 };
//    std::string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
//    for (auto d : Reverse<limbs>(str)) {
//        llimb carry = 0;
//        for (auto& s : result) {
//            llimb value = (static_cast<llimb>(s) << 32) + carry;
//            s = value % big_base;
//            carry = value / big_base;
//        }
//        if (carry > big_base) {
//            result.push_back(static_cast<limb>(carry % big_base));
//            result.push_back(static_cast<limb>(carry / big_base));
//        }
//        else if (carry)
//            result.push_back(static_cast<limb>(carry));
//
//        carry = d;
//        if (carry) {
//            for (auto& r : result) {
//                llimb value = r + carry;
//                r = value % big_base;
//                carry = value / big_base;
//                if (!carry)
//                    break;
//            }
//            if (carry)
//                result.push_back(static_cast<limb>(carry));
//        }
//    }
//    auto r = result.rbegin();
//    stream << *r;
//    std::advance(r, 1);
//
//    for (; r != result.rend(); ++r) {
//        std::string value = std::to_string(*r);
//        stream << std::string(expLimbLen[base - 2] - value.size(), '0');
//        stream << *r;
//    }
//}
//std::string bigint::bigint_t::toString_q(unsigned short base) const
//{
//    std::stringstream outputStream;
//    output(outputStream, base);
//    return outputStream.str();
//}


#pragma region Basic arithmetic functions

#pragma region Addition / Subtraction


void bigint::bigint_t::add(limbs::iterator sum,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{

    unsigned char carry = 0;
#ifndef _bigint_x86_x64_ARCH

    auto addChars = [&carry](limb v1, limb v2) -> limb
    {
        llimb sum = static_cast<llimb>(v1) + static_cast<llimb>(v2) + carry;
        if (sum >= (1ULL << 32)) {
            sum -= (1ULL << 32);
            carry = 1;
        }
        else
            carry = 0;
        return static_cast<limb>(sum);
    };
#endif
    auto s1 = str1_begin, s2 = str2_begin;
    for (; s1 != str1_end && s2 != str2_end; ++s1, ++s2, std::advance(sum, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, *s1, *s2, &(*sum));
#else
        *sum = addChars(*s1, *s2);
#endif
        
    }

    for (; s1 != str1_end; ++s1, std::advance(sum, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, *s1, 0, &(*sum));
#else
        *sum = addChars(*s1, 0);
#endif
    }
    for (; s2 < str2_end; ++s2, std::advance(sum, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, 0, *s2, &(*sum));
#else
        *sum = addChars(0, *s2);
#endif
    }

    if (carry)
        *sum = carry;
}

void bigint::bigint_t::addInto(limbs::iterator into,
    limbs::const_iterator     str_begin,
    limbs::const_iterator     str_end)
{
    unsigned char carry = 0;
#ifdef _bigint_x86_x64_ARCH
    for (auto s = str_begin; s != str_end; ++s, std::advance(into, 1))
        carry = _addcarry_u32(carry, *into, *s, &(*into));
#else
    auto addLimbs = [&carry](limb v1, limb v2) -> limb
    {
        llimb sum = static_cast<llimb>(v1) + static_cast<llimb>(v2) + carry;
        if (sum >= (1ULL << 32)) {
            sum -= (1ULL << 32);
            carry = 1;
        }
        else
            carry = 0;
        return static_cast<limb>(sum);
    };
    for (auto s = str_begin; s != str_end; ++s, std::advance(into, 1))
        * into = addLimbs(*into, *s);
#endif
    if (carry) {
        while (*into == static_cast<limb>((1ULL << 32) - 1)) {
            *into = 0;
            ++into;
        }
        *into += 1;
    }
}


void bigint::bigint_t::subtract(limbs::iterator delta,
                      limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                      limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    unsigned char borrow = 0;
#ifndef _bigint_x86_x64_ARCH
    auto subLimbs = [&carry] (limb v1, limb v2) -> limb
    {
        int64_t diff = static_cast<int64_t>(v1) - static_cast<int64_t>(v2) + carry;
        if (diff < 0) {
            diff += (1LL << 32);
            carry = -1;
        }
        else
            carry = 0;
        return static_cast<limb>(diff);
    };
#endif
    auto s1 = str1_begin, s2 = str2_begin;
    for (; s1 != str1_end && s2 != str2_end; ++s1, ++s2, std::advance(delta, 1)) {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, *s1, *s2, &(*delta));
#else
        *from = subLimbs(*s1, *s2);
#endif
    }

    for (; s1 != str1_end; ++s1, std::advance(delta, 1)) {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, *s1, 0, &(*delta));
#else
        *delta = subLimbs(*s1, 0);
#endif
    }
        
    for (; s2 != str2_end; ++s2, std::advance(delta, 1))
    {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, 0, *s2, &(*delta));
#else
        *delta = subLimbs(0, *s2);
#endif
    }
}

void bigint::bigint_t::subtractFrom(limbs::iterator       from,
    limbs::const_iterator str_begin,
    limbs::const_iterator str_end)
{
    unsigned char borrow = 0;
#ifdef _bigint_x86_x64_ARCH
    for (auto s = str_begin; s != str_end; ++s, std::advance(from, 1))
        borrow = _subborrow_u32(borrow, *from, *s, &(*from));
#else
    auto subLimbs = [&borrow](limb v1, limb v2) -> limb
        {
            int64_t diff = static_cast<int64_t>(v1) - static_cast<int64_t>(v2) - borrow;
            if (diff < 0) {
                diff += (1LL << 32);
                borrow = 1;
}
            else
                borrow = 0;
            return static_cast<limb>(diff);
        };
    for (auto s = str_begin; s != str_end; ++s, std::advance(from, 1))
        * from = subLimbs(*from, *s);
#endif
    if (borrow) {
        while (*from == 0) {
            *from = static_cast<limb>((1ULL << 32) - 1);
            ++from;
        }
        *from -= borrow;
    }
}


#pragma endregion


#pragma region Multiplication


limbs bigint::bigint_t::multiply(const limbs& str1, const limbs& str2)
{
    size_t str1_len = str1.size(), str2_len = str2.size();
    limbs product; product.resize(str1_len + str2_len, 0);
    mult(product.begin(), product.end(), str1.begin(), str1.end(), str2.begin(), str2.end());   
    trim(product);
    return product;
}

void bigint::bigint_t::mult(limbs::iterator       product   , limbs::iterator       product_end,
                  limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                  limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    constexpr size_t karatsubaThreshold = 128,
                     toomCookThreshold  = 128;

    ptrdiff_t str1_len = std::distance(str1_begin, str1_end),
              str2_len = std::distance(str2_begin, str2_end),
              calc_len = std::min(str1_len, str2_len);

    /*    if (calc_len >= toomCookThreshold)
        multiply_toom3(product, product_end, str1, 0, str1.size(), str2, 0,str2.size());
    else */
    if (calc_len >= karatsubaThreshold) {
        mult_karatsuba(product, product_end, str1_begin, str1_end, str2_begin, str2_end);
        return;

        //Bugged (?) version below
        if (str1_len == str2_len)
            mult_karatsuba_bugged(product, product_end, str1_begin, str1_end, str2_begin, str2_end);
        else if (str1_len > str2_len) {
            auto str1_mid = str1_begin;       auto product_mid1 = product;
            std::advance(str1_mid, calc_len); std::advance(product_mid1, calc_len);
            auto product_mid2 = product_mid1;
            std::advance(product_mid2, calc_len);
            mult_karatsuba_bugged(product, product_mid2, str1_begin, str1_mid, str2_begin, str2_end);
            mult(product_mid1, product_end, str1_mid, str1_end, str2_begin, str2_end);
        }
        else {
            auto str2_mid = str2_begin;       auto product_mid1 = product;
            std::advance(str2_mid, calc_len); std::advance(product_mid1, calc_len);
            auto product_mid2 = product_mid1;
            std::advance(product_mid2, calc_len);
            mult_karatsuba_bugged(product, product_mid2, str1_begin, str1_end, str2_begin, str2_mid);
            mult(product_mid1, product_end, str1_begin, str1_end, str2_mid, str2_end);
        }
    }
    else
        mult_vanilla(product, product_end, str1_begin, str1_end, str2_begin, str2_end);
}


void bigint::bigint_t::mult_vanilla(limbs::iterator       product   , limbs::iterator       product_end,
                          limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                          limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    limb carry = 0;
    auto multChars = [&carry](auto v1, auto v2, auto e) -> limb
    {
        llimb product = static_cast<llimb>(v1) * static_cast<llimb>(v2) + carry + e;
        carry = static_cast<limb>(product >> 32);
        return static_cast<limb>(product);
    };
    //auto multChars = [&carry](auto v1, auto v2, auto& e) -> void
    //    {
    //        llimb product = static_cast<llimb>(v1) * static_cast<llimb>(v2) + carry + e;
    //        carry = static_cast<limb>(product >> 32);
    //        z = static_cast<limb>(product);
    //    };

    for (auto s1 = str1_begin; s1 != str1_end; ++s1, std::advance(product, 1)) {
        carry = 0;
        auto op1 = *s1;
        auto prod_iter = product;
        for (auto s2 = str2_begin; s2 != str2_end; ++s2, std::advance(prod_iter, 1)) {
            auto& existing = *prod_iter;
            existing = multChars(op1, *s2, existing);
        }
        *prod_iter += carry;
    }
}


void bigint::bigint_t::mult_karatsuba(limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    ptrdiff_t len1 = std::distance(str1_begin, str1_end),
              len2 = std::distance(str2_begin, str2_end),
              splitO = std::min(len1, len2) - std::min(len1, len2) / 2, // Position of split of operands (half the smaller length, rounded up if needed).
              splitP = 2 * splitO;                                      // Position of split of product
#ifdef _DEBUG
    //if (len1 != len2)
    //    throw std::runtime_error("The length is supposed to be the same for the 2 input parameters.");
    //if (len <= 1)
    //    throw std::runtime_error("The parameters passed to the karatsuba algorithm are too small.");
    if (std::distance(product, product_end) < len1 + len2)
        throw std::runtime_error("The output parameter is too small to store the product of the 2 passed operands.");
#endif
    auto str1_mid = str1_begin, str2_mid = str2_begin;
    std::advance(str1_mid, splitO); std::advance(str2_mid, splitO);
    auto product_mid = product, product_midSum = product, product_mid2 = product;
    std::advance(product_mid, splitP);    std::advance(product_midSum, splitO);    std::advance(product_mid2, splitO);

    // product(2^0) <- b*d
    mult(product, product_mid, str1_begin, str1_mid, str2_begin, str2_mid);
    // product(2^2k) <- a*c
    mult(product_mid, product_end, str1_mid, str1_end, str2_mid, str2_end);

    limbs a_b, c_d, intermediate, intermediateSum;
    a_b.resize(static_cast<size_t>(len1 - splitO + 1), 0); c_d.resize(static_cast<size_t>(len2 - splitO + 1), 0); intermediate.resize(static_cast<size_t>(len1 + len2 - splitP + 2), 0); intermediateSum.resize(static_cast<size_t>(std::max(len1 + len2 - splitP, splitP) + 1), 0);

    add(a_b.begin(), str1_begin, str1_mid, str1_mid, str1_end);
    add(c_d.begin(), str2_begin, str2_mid, str2_mid, str2_end);
    mult(intermediate.begin(), intermediate.end(), a_b.cbegin(), a_b.cend(), c_d.cbegin(), c_d.cend());
    
    //a*c + b*d
    add(intermediateSum.begin(), product, product_mid, product_mid, product_end);

    addInto(product_midSum, intermediate.cbegin(), intermediate.cend());
    subtractFrom(product_midSum, intermediateSum.cbegin(), intermediateSum.cend());

    //print("result", product, product_end);
}


void bigint::bigint_t::mult_karatsuba_bugged(limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    size_t len = static_cast<size_t>(std::distance(str1_begin, str1_end)), SplitOperands = len / 2, SplitProduct = 2 * SplitOperands;
#ifdef _DEBUG
    size_t len2 = std::distance(str2_begin, str2_end), lenP = len + len2;
    if (len != len2)
        throw std::runtime_error("The length is supposed to be the same for the 2 input parameters.");
    if (len <= 1)
        throw std::runtime_error("The parameters passed to the karatsuba algorithm are too small.");
    if (static_cast<size_t>(std::distance(product, product_end)) < len + len2)
        throw std::runtime_error("The output parameter is too small to store the product of the 2 passed operands.");
#endif
    auto str1_mid = str1_begin, str2_mid = str2_begin; std::advance(str1_mid, SplitOperands); std::advance(str2_mid, SplitOperands);
    auto product_mid1 = product; std::advance(product_mid1, SplitOperands); auto product_mid2 = product_mid1; std::advance(product_mid2, SplitOperands);

    limbs ac, bd; ac.resize(2 * len - SplitProduct, 0); bd.resize(SplitProduct, 0);

    // product(2^0) <- b*d
    mult(bd.begin(), bd.end(), str1_begin, str1_mid, str2_begin, str2_mid);
    // product(2^2k) <- a*c
    mult(ac.begin(), ac.end(), str1_mid, str1_end, str2_mid, str2_end);

    addInto(product, bd.begin(), bd.end());
    addInto(product_mid2, ac.begin(), ac.end());

    limbs a_b, c_d, intermediate;
    a_b.resize(len - SplitOperands + 1, 0); c_d.resize(len - SplitOperands + 1, 0); intermediate.resize(2 * (len - SplitOperands) + 2, 0);

    // product(2^k) <- - a*c - b * d
    subtractFrom(product_mid1, ac.cbegin(), ac.cend());
    subtractFrom(product_mid1, bd.cbegin(), bd.cend());

    // product(2^k) <- (a + b) * (c + d)
    add(a_b.begin(), str1_begin, str1_mid, str1_mid, str1_end);
    add(c_d.begin(), str2_begin, str2_mid, str2_mid, str2_end);
    mult(intermediate.begin(), intermediate.end(), a_b.cbegin(), a_b.cend(), c_d.cbegin(), c_d.cend());
    addInto(product_mid1, intermediate.cbegin(), intermediate.cend());

}

void bigint::bigint_t::mult_toom3(limbs::iterator       product,    limbs::iterator       product_end,
                        limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                        limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    //size_t str1_len = str1.size(),
    //    str2_len = str2.size(),
    //    prod_len = str1_len + str2_len,
    //    word_len = (std::max(str1_len, str2_len) - 1) / 3 + 1;
    //
    //limbs m0, m1, m2, n0, n1, n2;
    //m0 = str1.substr(0, word_len);
    //m1 = str1.substr(word_len, word_len);
    //m2 = str1.substr(2 * word_len, word_len);
    //n0 = str2.substr(0, word_len);
    //n1 = str2.substr(word_len, word_len);
    //n2 = str2.substr(2 * word_len, word_len);

    //limbs p, p0, p1, p_1, p_2, pInf;
    //add(p, m0, m2);
    //p0 = m0;
    //add(p1, p, m1);
    //subtract(p_1, p, m1);
    //add(p_2, p_1, m2);
    //shiftLeft(p_2, 1);
    //subtract(p_2, p_2, m0);

    //limbs q, q0, q1, q_1, q_2, qInf;
    //add(q, n0, n2);
    //q0 = n0;
    //add(q1, p, n1);
    //subtract(q_1, q, n1);
    //add(q_2, q_1, n2);
    //shiftLeft(q_2, 1);
    //subtract(q_2, q_2, n0);

    //limbs r, r0, r1, r_1, r_2, rInf;

}

void bigint::bigint_t::_multiply(limbs& product, const limbs& str1, const limbs& str2) {             // return arithmetic multiplication of str1*str2
    size_t str1_len = str1.size(),
           str2_len = str2.size();
    if (str1_len == 0 || str2_len == 0) {
        product = limbs(1, 0);
        return;
    }
    if (str2_len > str1_len) {
        _multiply(product, str2, str1);
        return;
    }

    size_t prod_len = str1_len + str2_len;
    product.resize(prod_len, 0);

    auto transferCarry = [&product, prod_len] (size_t from, auto carry) {
        for (size_t p = from; p < prod_len && carry > 0; ++p) {
            carry += product[p];
            product[p] = static_cast<limb>(carry);
            carry = carry >> 32;
        }
    };

    for (size_t k = prod_len - 2; k >= str1_len; --k) {
        for (size_t i = k - str2_len; i <= k && i < str2_len; ++i) {
            llimb value = static_cast<llimb>(str1[k - i]) * static_cast<llimb>(str2[i]) + product[k],
                     carry = value >> 32;

            product[k] = static_cast<limb>(value);
            transferCarry(k + 1, carry);
        }
    }

    for (size_t k = str1_len - 1; k >= str2_len; --k) {
        {
            llimb value = static_cast<llimb>(str1[k]) * static_cast<llimb>(str2[0]),
                     carry = value >> 32;

            product[k] = static_cast<limb>(value);
            transferCarry(k + 1, carry);
        }
        for (size_t i = k - str2_len + 1; i <= k && i < str2_len; ++i) {
            llimb value = static_cast<llimb>(str1[k - i]) * static_cast<llimb>(str2[i]) + product[k],
                     carry = value >> 32;

            product[k] = static_cast<limb>(value);
            transferCarry(k + 1, carry);
        }
    }

    for (size_t k = str2_len - 1; k > 0; --k) {
        {
            llimb value1 = static_cast<llimb>(str1[k]) * static_cast<llimb>(str2[0]),
                     value2 = static_cast<llimb>(str1[0]) * static_cast<llimb>(str2[k]),
                     carry1 = value1 >> 32,
                     carry2 = value2 >> 32;

            product[k] = static_cast<limb>(value1 + value2);
            transferCarry(k + 1, carry1);
            transferCarry(k + 1, carry2);
        }
        for (size_t i = 1; i < k; ++i) {
            llimb value = static_cast<llimb>(str1[k - i]) * static_cast<llimb>(str2[i]) + product[k],
                     carry = value >> 32;

            product[k] = static_cast<limb>(value);
            transferCarry(k + 1, carry);
        }
    }

    {
        llimb value = static_cast<llimb>(str1[0]) * static_cast<llimb>(str2[0]),
                 carry = value >> 32;

        product[0] = static_cast<limb>(value);
        transferCarry(1, carry);

    }
}

#pragma endregion

#pragma region Division/mod functions

void bigint::bigint_t::divide(limbs& quotient, limbs& remainder, const limbs& str1, const limbs& str2)
{
    size_t str1_len = str1.size(),
           str2_len = str2.size();

    if (str2_len == 0 || std::all_of(str2.cbegin(), str2.cend(), [](limb l) { return l == 0; }))
        throw std::runtime_error("Division by 0");

    if (str2_len == 1) {
        //auto dividend = std::make_reverse_iterator(str1.end());
        //auto q = std::make_reverse_iterator(quotient); std::advance(q, -str1_len);
        //llimb r = 0;
        //for (; dividend != std::make_reverse_iterator(str1.begin()) && q != std::make_reverse_iterator(quotient); ++dividend, ++q) {
        //    *q = static_cast<limb>((b * r + *dividend) / str2[0]);
        //    r = r * b + *dividend - *q * *str2[0];
        //}
        //*remainder = static_cast<limb>(r);
    }

    if (str2_len > str1_len || compare(str1, str2) < 0) {
        quotient = { 0 };
        remainder = str2;
        return;
    }

    unsigned long requiredShift = 0;
    auto top_divisor_limb = std::make_reverse_iterator(str2.end());
    for (; top_divisor_limb != std::make_reverse_iterator(str2.begin()); ++top_divisor_limb) {
        if (*top_divisor_limb > 0)
            break;
    }
#ifdef _bigint_x86_x64_ARCH
    requiredShift = _lzcnt_u32(*top_divisor_limb);
#else
    limb v = *top_divisor_limb;
    while (v > 1) {
        v = v >> 1;
        requiredShift += 1;
    }
    requiredShift = 31 - requiredShift;
#endif

    if (requiredShift == 0) {
        size_t quotient_len = str1_len - str2_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(str2_len + 1, 0);

        div(quotient.begin(), remainder, str1.cbegin(), str1.cend(), str2.cbegin(), str2.cend());
    }
    else {
        limbs shiftedDividend(str1), shiftedDivisor(str2);
        shiftLeft(shiftedDividend, requiredShift);
        shiftLeft(shiftedDivisor, requiredShift);
        str1_len = shiftedDividend.size(), str2_len = shiftedDivisor.size();
        size_t quotient_len = str1_len - str2_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(str2_len + 1, 0);

        div(quotient.begin(), remainder, shiftedDividend.cbegin(), shiftedDividend.cend(), shiftedDivisor.cbegin(), shiftedDivisor.cend());

        shiftRight(remainder, requiredShift);
    }


    trim(quotient);
}

void bigint::bigint_t::div(limbs::iterator       quotient      , limbs&      remainder,
                 limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                 limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end)
{

    div_vanilla(quotient, remainder, dividend_begin, dividend_end, divisor_begin, divisor_end);
}

void bigint::bigint_t::div_vanilla(limbs::iterator       quotient      , limbs&       remainder,
                         limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                         limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end)
{
    //auto print = [](auto name, const bigint::bigint_t& value) {
    //    std::string sValue = value.toString_slow();
    //    OutputDebugStringA(name);
    //    OutputDebugString(L"(");
    //    OutputDebugStringA(sValue.c_str());
    //    OutputDebugString(L"): ");
    //    for (auto i = value.str.crbegin(); i != value.str.crend(); ++i)
    //        OutputDebugString((std::to_wstring(*i) + L" ").c_str());
    //    OutputDebugString(L"\n");
    //};

    bigint_t dividend, divisor;
    dividend.str.resize(std::distance(dividend_begin, dividend_end), 0);
    divisor.str.resize(std::distance(divisor_begin, divisor_end), 0);
    std::copy(dividend_begin, dividend_end, dividend.str.begin());
    std::copy(divisor_begin, divisor_end, divisor.str.begin());

    //print("dividend", dividend);
    //print("divisor ", divisor);

    auto shift = [](limbs::iterator begin, limbs::iterator end, limb value) -> void {
        for (auto i1 = std::make_reverse_iterator(end), i2 = i1 + 1; i2 != std::make_reverse_iterator(begin); ++i1, ++i2) {
            *i1 = *i2;
        }
        *begin = value;
    };

    limbs one{ 1 }, progress{ 1 };
    ptrdiff_t dividend_len = std::distance(dividend_begin, dividend_end),
              divisor_len  = std::distance(divisor_begin, divisor_end);
    if (dividend_len > divisor_len)
        shiftLeft(progress, static_cast<size_t>((dividend_len - divisor_len) * 32 - 1));

    auto d = std::make_reverse_iterator(dividend_end);
    std::advance(d, divisor_len - 1);
    std::copy(d.base(), dividend_end, remainder.begin());
    //{
    //    bigint_t r; r.str = remainder;
    //    print("remainder", r);
    //}

    llimb significant_div_digit = *(divisor_end - 1);
    auto quotient_end = quotient;

    for (; d != std::make_reverse_iterator(dividend_begin); ++d) {
        shift(remainder.begin(), remainder.end(), *d);
        //{
        //    bigint_t r; r.str = remainder;
        //    print("remainder", r);
        //}
        llimb value;
        //if (remainder[remainder.size() - 1] == 0)
        //    value = (static_cast<llimb>(remainder[remainder.size() - 2]) << 32) + remainder[remainder.size() - 3];
        //else
            value = (static_cast<llimb>(remainder[remainder.size() - 1]) << 32) + remainder[remainder.size() - 2];
        llimb result = value / significant_div_digit;
        if (result > 0) {
            if (result <= 4)
                result = 0;
            else
                result -= 4 ;
            auto r = remainder.begin(); auto s = divisor_begin; llimb carry = 0;
            for (; s != divisor_end; ++r, ++s) {
                value = result * *s + carry;
                unsigned short c = (*r >= static_cast<limb>(value)) ? 0 : 1;
                //sub1 = ();
                *r -= static_cast<limb>(value);
                carry = (value >> 32) + c;
            }
            while (carry > 0) {
                unsigned short c = (*r >= static_cast<limb>(carry)) ? 0 : 1;
                *r -= static_cast<limb>(carry);
                ++r;
                carry = c;
            }
            while (compare(remainder.begin(), remainder.end(), divisor_begin, divisor_end) >= 0) {
                //{
                //    bigint_t r; r.str = remainder;
                //    print("remainder", r);
                //}
                result += 1;
                subtract(remainder.begin(), remainder.begin(), remainder.end(), divisor_begin, divisor_end);
            }
            //{
            //    bigint_t r; r.str = remainder;
            //    print("remainder", r);
            //}
        }
        ++quotient_end;
        shift(quotient, quotient_end, static_cast<limb>(result));
        //{
        //    bigint_t q;
        //    q.str.resize(std::distance(quotient, quotient_end));
        //    std::copy(quotient, quotient_end, q.str.begin());
        //    print("quotient  ", q);
        //}
        shiftRight(progress, 32);
    }
}

void bigint::bigint_t::mod(limbs& quotient, limbs& remainder, limbs str1, limbs str2)
{
    size_t str1_len = str1.size(),
        str2_len = str2.size();

    if (str2_len == 0 || std::all_of(str2.cbegin(), str2.cend(), [](limb l) { return l == 0; }))
        throw std::runtime_error("Division by 0");

    if (str2_len == 1) {
        //auto dividend = std::make_reverse_iterator(str1.end());
        //auto q = std::make_reverse_iterator(quotient); std::advance(q, -str1_len);
        //llimb r = 0;
        //for (; dividend != std::make_reverse_iterator(str1.begin()) && q != std::make_reverse_iterator(quotient); ++dividend, ++q) {
        //    *q = static_cast<limb>((b * r + *dividend) / str2[0]);
        //    r = r * b + *dividend - *q * *str2[0];
        //}
        //*remainder = static_cast<limb>(r);
    }

    if (str2_len > str1_len || compare(str1, str2) < 0) {
        quotient = { 0 };
        remainder = str2;
        return;
    }

    unsigned long requiredShift = 0;
    auto top_divisor_limb = std::make_reverse_iterator(str2.end());
    for (; top_divisor_limb != std::make_reverse_iterator(str2.begin()); ++top_divisor_limb) {
        if (*top_divisor_limb > 0)
            break;
    }
#ifdef _bigint_x86_x64_ARCH
    requiredShift = _lzcnt_u32(*top_divisor_limb);
#else
    limb v = *top_divisor_limb;
    while (v > 1) {
        v = v >> 1;
        requiredShift += 1;
}
    requiredShift = 31 - requiredShift;
#endif

    if (requiredShift == 0) {
        size_t quotient_len = str1_len - str2_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(str2_len + 1, 0);

        div(quotient.begin(), remainder, str1.cbegin(), str1.cend(), str2.cbegin(), str2.cend());
    }
    else {
        limbs shiftedDividend(str1), shiftedDivisor(str2);
        shiftLeft(shiftedDividend, requiredShift);
        shiftLeft(shiftedDivisor, requiredShift);
        str1_len = shiftedDividend.size(), str2_len = shiftedDivisor.size();
        size_t quotient_len = str1_len - str2_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(str2_len + 1, 0);

        div(quotient.begin(), remainder, shiftedDividend.cbegin(), shiftedDividend.cend(), shiftedDivisor.cbegin(), shiftedDivisor.cend());

        shiftRight(remainder, requiredShift);
    }
    trim(remainder);
}

#pragma endregion

void bigint::bigint_t::shiftLeft(limbs& str, size_t value)
{
    size_t subIntValue = value & 31;

    limb carry = 0;
    for (auto s = str.begin(); s != str.end(); std::advance(s, 1)) {
//#ifdef _bigint_x86_x64_ARCH

//#else
        llimb a = (static_cast<llimb>(*s) << subIntValue) + carry;
        *s = static_cast<limb>(a);
        carry = a >> 32;
//#endif
    }

    if (carry)
        str.push_back(carry);
    if (value >= 32)
        str.insert(0, value >> 5, 0);
}

void bigint::bigint_t::shiftRight(limbs& str, size_t value)
{
    if ((value >> 5) >= str.size()) {
        str = limbs(1, 0);
        return;
    }

    str.erase(0, value >> 5);

    size_t subIntValue = value & 31;

    unsigned int carry = 0;
    for (auto s = str.rbegin(); s != str.rend(); std::advance(s, 1)) {
        auto shiftValue = *s & ((1 << subIntValue) - 1);
        *s = (*s >> subIntValue) + (carry << (32 - subIntValue));
        carry = shiftValue;
    }



    trim(str);
}


#pragma endregion

#pragma region Comparison functions

int bigint::bigint_t::compare(limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    int result = 0;
    auto s1 = str1_begin, s2 = str2_begin;
    for (; s1 != str1_end && s2 != str2_end; ++s1, ++s2) {
        if (*s1 > *s2)
            result = 1;
        else if (*s1 < *s2)
            result = -1;
    }

    for (; s1 != str1_end; ++s1) {
        if (*s1 != 0)
            return 1;
    }
    for (; s2 != str2_end; ++s2) {
        if (*s2 != 0)
            return -1;
    }

    return result;
}

int bigint::bigint_t::compare(const limbs& str1, const limbs& str2)
{
    auto str1_len = str1.size(), str2_len = str2.size();
    if (str1_len < str2_len)
        return -1;
    else if (str1_len > str2_len)
        return 1;
    else {
        for (auto s1 = str1.rbegin(), s2 = str2.rbegin(); s1 != str1.rend(); ++s1, ++s2) {
            if (*s1 < *s2)
                return -1;
            else if (*s1 > *s2)
                return 1;
        }
    }
    return 0;
}

#pragma endregion

void bigint::bigint_t::trim(limbs& s) {                         // function to remove zeros
    size_t len = s.size(), i = len - 1;
    while (i > 0 && s[i] == 0)
        --i;
    s.resize(i + 1);
}

#pragma region Advanced mathematical functions


limbs bigint::bigint_t::sqrt(limbs s) {
    //if(s[0] == '-')
    //    return s;
    //if(s == "0")
    //    return "0";
    //unsigned long long int s_len = s.size();
    //limbs ans;
    //limbs mid = "";
    //limbs high, low, square;
    //unsigned long long int ans_len = s_len >> 1;
    //if(s_len & 1) {
    //    low = pow("10", std::to_string(ans_len));
    //    high = pow("10", std::to_string(ans_len+1));
    //} else {
    //    low = pow("10", std::to_string(ans_len-1));
    //    high = pow("10", std::to_string(ans_len));
    //}
    //limbs prev;
    //while(1) {
    //    limbs sum;
    //    add(sum, high, low);
    //    mid = divide(sum, "2");
    //    square = multiply(mid, mid);
    //    add(sum, square, mid);
    //    if(prev == mid || (maximum(sum, s) == sum && maximum(square, s) == s ) || high == low) {
    //        break;
    //    }
    //    if(maximum(square, s) == s) {
    //       low = mid;
    //    } else if(maximum(square, s) == square) {
    //         high = mid;
    //    }
    //    prev = mid;
    //}
    //ans = mid;
    //return ans;
    return limbs();
}

void bigint::bigint_t::swap(limbs& str1, limbs& str2) {
    std::swap(str1, str2);
}

limbs bigint::bigint_t::gcd(limbs str1, limbs str2) {
    if(compare(str1, str2) < 0)
        return gcd(str2, str1);
    while (compare(str2, limbs(1, 0)) > 0) {
        limbs quotient, remainder;
        mod(quotient, remainder, str1, str2);
        std::swap(str1, str2);
        std::swap(str2, remainder);
    }
    return str1;
}

limbs bigint::bigint_t::lcm(limbs str1, limbs str2) {
    limbs quotient, remainder;
    divide(quotient, remainder, multiply(str1, str2), gcd(str1, str2));
    return quotient;
}


bool bigint::bigint_t::isPrime(limbs str) {
    if (!(str[0] & 1))
        return false;
    {
        limbs quotient, remainder;
        mod(quotient, remainder, str, { 3 });
        if (remainder == limbs(1, 0))
            return false;
    }

    // Now that 2 and 3 are out of the way, test all possible factors of the form 6n-1 and 6n+1
    limbs two = { 2 }, four = { 4 },
          sqrt = bigint::bigint_t::sqrt(str);
    for(limbs i = bigint_t(7LL).str; compare(sqrt, i) > 0; add(i.begin(), i.cbegin(), i.cend(), two.cbegin(), two.cend())) {
        limbs quotient, remainder;
        mod(quotient, remainder, str, i);
        if(remainder == limbs(1, 0))
            return false;
        add(i.begin(), i.cbegin(), i.cend(), four.cbegin(), four.cend());
        mod(quotient, remainder, str, i);
        if (remainder == limbs(1, 0))
            return false;
    }
    return true;
}

#pragma endregion

#pragma warning(pop)


#endif
