/****************************************************************************************/
/*                                                                                      */
/*    bigint.h                                                                          */
/*                                                                                      */
/*    bigint Library for C++                                                            */
/*                                                                                      */
/*    MIT License                                                                       */
/*    Copyright (c) 2023 François-Xavier David                                          */
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
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

// Big Integer Class

#include <iostream>

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

class bigint {
public:
    signed char sign;
    limbs str;

    static void trim(limbs&);
    static limbs trimmed(limbs);

    static int compare(limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                       limbs::const_iterator str2_begin, limbs::const_iterator str2_end);

    static int compare(const limbs& str1, const limbs& str2);

    static void add         (limbs::iterator,
                             limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                             limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
    static void addInto     (limbs::iterator       into,
                             limbs::const_iterator str_begin, limbs::const_iterator str_end);
    static void subtract    (limbs::iterator,
                             limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
                             limbs::const_iterator str2_begin, limbs::const_iterator str2_end);
    static void subtractFrom(limbs::iterator       from,
                             limbs::const_iterator str_begin, limbs::const_iterator str_end);
    static void _multiply   (limbs&, const limbs&, const limbs&);
    static void shiftLeft   (limbs&, size_t);
    static void shiftRight  (limbs&, size_t);


    static limbs multiply     (const limbs& str1, const limbs& str2);
    static void mult          (limbs::iterator       product_begin, limbs::iterator       product_end,
                               limbs::const_iterator str1_begin   , limbs::const_iterator str1_end,
                               limbs::const_iterator str2_begin   , limbs::const_iterator str2_end);
    static void mult_vanilla  (limbs::iterator       product_begin, limbs::iterator       product_end,
                               limbs::const_iterator str1_begin   , limbs::const_iterator str1_end,
                               limbs::const_iterator str2_begin   , limbs::const_iterator str2_end);
    static void mult_karatsuba(limbs::iterator       product_begin, limbs::iterator       product_end,
                               limbs::const_iterator str1_begin   , limbs::const_iterator str1_end,
                               limbs::const_iterator str2_begin   , limbs::const_iterator str2_end);
    static void mult_karatsuba_bugged(limbs::iterator product_begin, limbs::iterator       product_end,
                               limbs::const_iterator str1_begin   , limbs::const_iterator str1_end,
                               limbs::const_iterator str2_begin   , limbs::const_iterator str2_end);
    static void mult_toom3    (limbs::iterator       product_begin, limbs::iterator       product_end,
                               limbs::const_iterator str1_begin   , limbs::const_iterator str1_end,
                               limbs::const_iterator str2_begin   , limbs::const_iterator str2_end);

    //static void multiply_schonhageStrassen(limbs&, const limbs& str1, const limbs& str2);
    static void divide    (limbs& quotient, limbs& remainder, const limbs& str1, const limbs& str2);
    static void div        (limbs::iterator       quotient      , limbs&                remainder,
                            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                            limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end);
    static void div_vanilla(limbs::iterator       quotient      , limbs&                remainder,
                            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                            limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end);
    static void div_mnu    (limbs::iterator       quotient      , limbs::iterator       remainder,
                            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                            limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end);

    static limbs mod              (limbs&, limbs&, limbs, limbs);

    static limbs shortDivide      (limbs, unsigned long long int);
    static limbs maximum          (limbs, limbs);
    static bool is_maximum        (limbs, limbs);
    static bool is_strictlyMaximum(limbs, limbs);
    static limbs minimum          (limbs, limbs);
    static bool is_minimum        (limbs, limbs);
    static bool is_strictlyMinimum(limbs, limbs);
    static bool is_bigint         (const std::string&);


    static limbs pow        (limbs, limbs);
    static limbs sqrt       (limbs);
    static limbs log2       (limbs);
    static limbs log10      (limbs);
    static limbs logwithbase(limbs, limbs);
    static limbs antilog2   (limbs);
    static limbs antilog10  (limbs);
    static void  swap       (limbs&, limbs&);
    static limbs reverse    (limbs);
    static limbs gcd        (limbs, limbs);
    static limbs lcm        (limbs, limbs);
    static limbs fact       (limbs);
    //static limbs factorial  (size_t);
    static bool  isPrime    (limbs);


public:
    bigint() :
        sign(1),
        str(1, 0)
    {
    }
    bigint(std::string source) :
        sign(source[0] == '-' ? -1 : 1)
    {
        str.push_back(0);

        auto d = source.begin();
        if (sign < 0)
            std::advance(d, 1);
        for (; d != source.end(); ++d) {
            if (*d < '0' || *d > '9')
                throw std::runtime_error("Invalid Big Integer has been fed.");
         
            llimb carry = static_cast<llimb>(*d) - '0';
            for (auto i = str.begin(); i != str.end(); std::advance(i, 1)) {
                llimb value = static_cast<llimb>(*i) * 10 + carry;
                *i = static_cast<limb>(value);
                carry = value >> 32;
            }
            if (carry)
                str.push_back(static_cast<limb>(carry));
        }
    }
    bigint(long long int n) :
        sign(n >= 0 ? 1 : -1)
    {
        n = sign * n;
        str.push_back(static_cast<limb>(n));
        if (n > (1LL << 32))
            str.push_back(static_cast<limb>(n >> 32));
    }
    bigint(const bigint& n) :
        sign(n.sign),
        str(n.str)
    {
    }

    void output(std::ostream& stream, unsigned short base = 10) const;
    void output_fast(std::ostream& stream, unsigned short base) const;
    std::string toString(unsigned short base = 10) const;

    friend std::ostream& operator << (std::ostream& stream, const bigint& n) {
       switch (stream.flags() & std::ios_base::basefield) {
       case stream.oct: n.output_fast(stream,  8); return stream;
       case stream.hex: n.output_fast(stream, 16); return stream;
       case stream.dec:
       default:         n.output     (stream, 10); return stream;
       }
    }
    friend std::istream& operator >> (std::istream& stream, bigint& n) {
        //stream >> n.str;
        return stream;
    }

    bigint operator + (bigint const& n) const {
        bigint ans;
        if (sign == n.sign) {
            ans.str.resize(std::max(str.size(), n.str.size()) + 1, 0);
            add(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
        }
        else {
            ans.str.resize(std::max(str.size(), n.str.size()));
            auto str1_len = str.size(), str2_len = n.str.size();
            if (str1_len > str2_len)
                ans.sign = sign;
            else if (str1_len < str2_len)
                ans.sign = n.sign;
            else {
                auto s1 = str.rbegin(), s2 = str.rbegin();
                for (; s1 != str.rend(); ++s1, ++s2) {
                    if (*s1 > *s2) {
                        ans.sign = sign;
                        break;
                    }
                    else if (*s1 < *s2) {
                        ans.sign = n.sign;
                        break;
                    }
                }
                if (s1 == str.rend()) {
                    ans.sign = 0;
                    ans.str = { 0 };
                    return ans;
                }
            }
            subtract(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
        }
        trim(ans.str);
        return ans;
    }
    friend bigint operator + (bigint const& n1, long long int n2) {
        return n1 + bigint(n2);
    }
    friend bigint operator + (long long int n1, bigint const& n2) {
        return bigint(n1) + n2;
    }
    bigint& operator += (bigint const& n) {
        if (sign == n.sign) {
            if (n.str.size() >= str.size())
               str.resize(n.str.size() + 1, 0);
            else if (str[str.size() - 1] == static_cast<limb>((1ULL <<32) - 1))
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

    bigint operator - (bigint const& n) const
    {
        bigint ans;
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
                ans.str.resize(std::max(str.size(), n.str.size()), 0);
                subtract(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
                ans.sign = -sign;
                break;
            }
            case 1: {
                ans.str.resize(std::max(str.size(), n.str.size()), 0);
                subtract(ans.str.begin(), str.cbegin(), str.cend(), n.str.cbegin(), n.str.cend());
                ans.sign = sign;
                break;
            }
            }
        }
        trim(ans.str);
        return ans;
    }
    friend bigint operator - (bigint const &n1, long long int n2)
    {
        return n1 - bigint(n2);
    }
    friend bigint operator - (long long int n1, bigint const &n2)
    {
        return bigint(n1) - n2;
    }
    bigint& operator -= (bigint const n) {
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

    bigint operator * (bigint const& n) const {
        bigint ans;
        ans.sign = sign * n.sign;
        ans.str = multiply(str, n.str);
        return ans;
    }
    friend bigint operator * (bigint const &n1, long long int n2)
    {
        return n1 * bigint(n2);
    }
    friend bigint operator * (long long int n1, bigint const &n2)
    {
        return bigint(n1) * n2;
    }

    bigint& operator *= (bigint const n) {
        sign *= n.sign;
        str = multiply(str, n.str);
        return *this;
    }

    bigint operator / (bigint const& n) const
    {
        limbs quotient, remainder;
        divide(quotient, remainder, str, n.str);
        bigint ans; ans.str = quotient;
        return ans;
    }
    friend bigint operator / (bigint const &n1, int n2)
    {
        return n1 * bigint(n2);
    }
    friend bigint operator / (int n1, bigint const &n2)
    {
        return bigint(n1) * n2;
    }
    bigint& operator /= (bigint const n)
    {
        limbs quotient, remainder;
        divide(quotient, remainder, str, n.str);
        str.swap(quotient);
        return *this;
    }


    bigint operator % (bigint const& n) const {
        bigint ans;
        ans.str = mod(str, n.str);
        return ans;
    }
    friend bigint operator % (bigint const &n1, int n2) {
        return n1 % bigint(n2);
    }
    friend bigint operator % (int n1, bigint const &n2) {
        return bigint(n1) % n2;
    }
    bigint& operator %= (bigint const n) {
        str = mod(str, n.str);
        return *this;
    }

    bigint operator<<(size_t value)
    {
        bigint ans(*this);
        shiftLeft(ans.str, value);
        return ans;
    }
    bigint operator>>(size_t value)
    {
        bigint ans(*this);
        shiftRight(ans.str, value);
        return ans;
    }


    bigint& operator ++ () {
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
    bigint  operator ++ (int v) {
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
    bigint& operator -- () {
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
    bigint  operator -- (int v) {
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

    bool operator ==(bigint const& n) const {
        return sign == n.sign && str == n.str;
    }
    bool operator > (bigint const& n) const {
        return compare(str, n.str) > 0;
    }
    bool operator < (bigint const& n) const {
        return compare(str, n.str) < 0;
    }
    bool operator >=(bigint const& n) const {
        return compare(str, n.str) >= 0;
    }
    bool operator <=(bigint const& n) const {
        return compare(str, n.str) <= 0;
    }


    bool operator !=(bigint const &n) {
        return str != n.str;
    }
#if __cplusplus >= 202002L
        std::strong_ordering operator<=>(bigint const& n)
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

        
        static bigint _big_max(bigint &a, bigint &b) {
            bigint ans;
            ans.str = maximum(a.str, b.str);
            return ans;
        }
        static bigint _big_min(bigint &a, bigint &b) {
            bigint ans;
            ans.str = minimum(a.str, b.str);
            return ans;
        }        
        static bigint _big_abs(bigint &a) {
            bigint ans(a);
            ans.sign = 1;
            return ans;
        }
        static bigint _big_pow(bigint &a, bigint &b) {
            bigint ans;
            ans.str = pow(a.str, b.str);
            return ans;
        }
        static bigint _big_sqrt(bigint &a) {
            bigint ans;
            ans.str = sqrt(a.str);
            return ans;
        }
        static bigint _big_log2(bigint &a) {
            bigint ans;
            ans.str = log2(a.str);
            return ans;
        }
        static bigint _big_log10(bigint &a) {
            bigint ans;
            ans.str = log10(a.str);
            return ans;
        }
        static bigint _big_logwithbase(bigint &a, bigint &b) {
            bigint ans;
            ans.str = logwithbase(a.str, b.str);
            return ans;
        }
        static bigint _big_antilog2(bigint &a) {
            bigint ans;
            ans.str = antilog2(a.str);
            return ans;
        }
        static bigint _big_antilog10(bigint &a) {
            bigint ans;
            ans.str = antilog10(a.str);
            return ans;
        }
        static void _big_swap(bigint &a, bigint &b) {
            swap(a.str, b.str);
        }
        static bigint _big_reverse(bigint &a) {
            bigint ans;
            ans.str = reverse(a.str);
            return ans;
        }
        static bigint _big_gcd(bigint &a, bigint &b) {
            bigint ans;
            ans.str = gcd(a.str, b.str);
            return ans;
        }
        static bigint _big_lcm(bigint &a, bigint &b) {
            bigint ans;
            ans.str = lcm(a.str, b.str);
            return ans;
        }
        static bigint _big_fact(bigint &a) {
            bigint ans;
            ans.str = fact(a.str);
            return ans;
        }
        static bigint factorial(size_t);
        static bool _big_isPrime(bigint &a) {
            return isPrime(a.str);
        }

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

    auto begin() const noexcept { using std::end;   return std::make_reverse_iterator(end(rng)); }
    auto end()   const noexcept { using std::begin; return std::make_reverse_iterator(begin(rng)); }
};


bool bigint::is_bigint(const std::string& s) {
    size_t start = 0;
    if (s[0] == '-')
        start = 1;
    for(size_t i = start; i < s.size(); ++i) {
        if(!((int)s[i] > 47 && (int)s[i] < 58))
            return false;
    }
    return true;
}

#include <algorithm>

void bigint::output(std::ostream& stream, unsigned short base) const
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

    size_t str_len = str.size();
    //if (str_len >= 128) {
    //    bigint big_base{ basePower[base - 2] };
    //    size_t half_len = str_len / 2 + 1;
    //    do {
    //        big_base *= big_base;
    //        half_len = half_len >> 1;
    //    } while (half_len > 1);
    //    limbs q, r;
    //    bigint::divide(q, r, str, big_base.str);
    //    trim(q); trim(r);
    //    bigint quotient, remainder;
    //    quotient.str = q; remainder.str = r;
    //    quotient.output(stream, base);
    //    auto sr = remainder.toString(base);
    //    if (r.size() * expLimbLen[base - 2] > sr.size())
    //       stream << std::string(r.size() * expLimbLen[base - 2] - sr.size(), 0) << sr;
    //}

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
        else if(carry)
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
        }
        if (carry)
            result.push_back(carry);
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
void bigint::output_fast(std::ostream& stream, unsigned short base) const
{
}

std::string bigint::toString(unsigned short base) const
{
    std::stringstream outputStream;
    output(outputStream, base);
    return outputStream.str();
}


#pragma region Basic arithmetic functions

#pragma region Addition / Subtraction


void bigint::add(limbs::iterator sum,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{

    char carry = 0;
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

void bigint::addInto(limbs::iterator       into,
                     limbs::const_iterator str_begin,
                     limbs::const_iterator str_end)
{
    unsigned char carry = 0;
#ifndef _bigint_x86_x64_ARCH
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
#endif
    for (auto s = str_begin; s != str_end; ++s, std::advance(into, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, *into, *s, &(*into));
#else
        *into = addLimbs(*into, *s);
#endif
    }
    while (carry) {
        if (*into == static_cast<limb>((1ULL << 32) - 1)) {
            *into = 0;
            ++into;
        }
        else {
            *into += carry;
            carry = 0;
        }
    }
}


void bigint::subtract(limbs::iterator delta,
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

void bigint::subtractFrom(limbs::iterator       from,
                          limbs::const_iterator str_begin,
                          limbs::const_iterator str_end)
{
    unsigned char borrow = 0;
#ifndef _bigint_x86_x64_ARCH
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
#endif
    for (auto s = str_begin; s != str_end; ++s, std::advance(from, 1)) {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, *from, *s, &(*from));
#else
        *from = subLimbs(*from, *s);
#endif
    }

    while (borrow) {
        if (*from < borrow) {
            *from = static_cast<limb>((1ULL << 32) - 1);
            ++from;
        }
        else {
            *from -= borrow;
            break;
        }
    }
}


#pragma endregion


#pragma region Multiplication


limbs bigint::multiply(const limbs& str1, const limbs& str2)
{
    size_t str1_len = str1.size(), str2_len = str2.size();
    limbs product; product.resize(str1_len + str2_len, 0);
    mult(product.begin(), product.end(), str1.begin(), str1.end(), str2.begin(), str2.end());   
    trim(product);
    return product;
}

void bigint::mult(limbs::iterator       product   , limbs::iterator       product_end,
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


void bigint::mult_vanilla(limbs::iterator       product   , limbs::iterator       product_end,
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


void bigint::mult_karatsuba(limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    ptrdiff_t len1 = std::distance(str1_begin, str1_end),
              len2 = std::distance(str2_begin, str2_end),
              splitO = std::min(len1, len2) - std::min(len1, len2) / 2, // Position of split of operands (half the smaller length, rounded up if needed).
              splitP = 2 * splitO;                                      // Position of split of product
#if _DEBUG
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
    a_b.resize(len1 - splitO + 1, 0); c_d.resize(len2 - splitO + 1, 0); intermediate.resize(len1 + len2 - splitP + 2, 0); intermediateSum.resize(std::max(len1 + len2 - splitP, splitP) + 1, 0);

    add(a_b.begin(), str1_begin, str1_mid, str1_mid, str1_end);
    add(c_d.begin(), str2_begin, str2_mid, str2_mid, str2_end);
    mult(intermediate.begin(), intermediate.end(), a_b.cbegin(), a_b.cend(), c_d.cbegin(), c_d.cend());
    
    //a*c + b*d
    add(intermediateSum.begin(), product, product_mid, product_mid, product_end);

    addInto(product_midSum, intermediate.cbegin(), intermediate.cend());
    subtractFrom(product_midSum, intermediateSum.cbegin(), intermediateSum.cend());

    //print("result", product, product_end);
}


void bigint::mult_karatsuba_bugged(limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
    limbs::const_iterator str2_begin, limbs::const_iterator str2_end)
{
    size_t len = std::distance(str1_begin, str1_end), SplitOperands = len / 2, SplitProduct = 2 * SplitOperands;
#if _DEBUG
    size_t len2 = std::distance(str2_begin, str2_end), lenP = len + len2;
    if (len != len2)
        throw std::runtime_error("The length is supposed to be the same for the 2 input parameters.");
    if (len <= 1)
        throw std::runtime_error("The parameters passed to the karatsuba algorithm are too small.");
    if (std::distance(product, product_end) < len + len2)
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

void bigint::mult_toom3(limbs::iterator       product,    limbs::iterator       product_end,
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

void bigint::_multiply(limbs& product, const limbs& str1, const limbs& str2) {             // return arithmetic multiplication of str1*str2
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

void bigint::divide(limbs& quotient, limbs& remainder, const limbs& str1, const limbs& str2)
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

void bigint::div(limbs::iterator       quotient      , limbs&      remainder,
                 limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                 limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end)
{

    div_vanilla(quotient, remainder, dividend_begin, dividend_end, divisor_begin, divisor_end);
}

void bigint::div_vanilla(limbs::iterator       quotient      , limbs&       remainder,
                         limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
                         limbs::const_iterator divisor_begin , limbs::const_iterator divisor_end)
{
 //   std::cerr << "Begin division" << std::endl;
    auto print = [](const std::string& variableName, limbs::const_iterator begin, limbs::const_iterator end) -> void {
        return;
        std::cerr << variableName << ':';
        for (auto s = variableName.size(); s < 12; ++s)
            std::cerr << ' ';
        for (auto i = begin; i != end; ++i)
        {
            std::cerr << std::setw(10) << *i << ' ';
        }
        std::cerr << '\n';
    };

    print("Dividend", dividend_begin, dividend_end);
    print("Divisor", divisor_begin, divisor_end);

    auto shift = [](limbs::iterator begin, limbs::iterator end, limb value) -> void {
        for (auto i1 = std::make_reverse_iterator(end), i2 = i1 + 1; i2 != std::make_reverse_iterator(begin); ++i1, ++i2) {
            *i1 = *i2;
        }
        *begin = value;
    };

    limbs one{ 1 }, progress{ 1 };
    size_t dividend_len = std::distance(dividend_begin, dividend_end),
           divisor_len  = std::distance(divisor_begin, divisor_end);
    shiftLeft(progress, (dividend_len - divisor_len) * 32 - 1);

    auto d = std::make_reverse_iterator(dividend_end);
    std::advance(d, divisor_len);
    std::copy(d.base(), dividend_end, remainder.begin());

    llimb significant_div_digit = *(divisor_end - 1);
    auto quotient_end = quotient;

    for (; d != std::make_reverse_iterator(dividend_begin); ++d) {
        shift(remainder.begin(), remainder.end(), *d);

        print("Remainder", remainder.begin(), remainder.end());

        llimb value  = (static_cast<llimb>(remainder[remainder.size() - 1]) << 32) + remainder[remainder.size() - 2],
                 result = value / significant_div_digit - 1;
        if (result > 0) {
            auto r = remainder.begin(); auto s = divisor_begin; llimb carry = 0;
            bool sub1 = false;
            for (; s != divisor_end; ++r, ++s) {
                value = result * *s + carry;
                short c = (*r >= static_cast<limb>(value)) ? 0 : 1;
                //sub1 = ();
                *r -= (static_cast<limb>(value) /*+ carry*/);
                carry = (value >> 32) + c;
            }
            *r -= carry;
            if (compare(remainder.begin(), remainder.end(), divisor_begin, divisor_end) >= 0) {
                result += 1;
                subtractFrom(remainder.begin(), divisor_begin, divisor_end);
            }
        }
        ++quotient_end;
        shift(quotient, quotient_end, result);
        print("Remainder", remainder.begin(), remainder.end());
        print("Quotient", quotient, quotient_end);

        shiftRight(progress, 32);
    }
//    std::cerr << "End division" << std::endl;
}

limbs bigint::mod(limbs& quotient, limbs& remainder, limbs str1, limbs str2)
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
    trim(remainder);
    return remainder;
}

#pragma endregion

void bigint::shiftLeft(limbs& str, size_t value)
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

void bigint::shiftRight(limbs& str, size_t value)
{
    if ((value >> 5) >= str.size()) {
        str = limbs(1, 0);
        return;
    }

    str.erase(0, value >> 5);

    size_t subIntValue = value & 31;

    auto carry = 0;
    for (auto s = str.rbegin(); s != str.rend(); std::advance(s, 1)) {
        auto shiftValue = *s & ((1 << subIntValue) - 1);
        *s = (*s >> subIntValue) + (carry << (32 - subIntValue));
        carry = shiftValue;
    }



    trim(str);
}


#pragma endregion

#pragma region Comparison functions

int bigint::compare(limbs::const_iterator str1_begin, limbs::const_iterator str1_end,
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

int bigint::compare(const limbs& str1, const limbs& str2)
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

limbs bigint::maximum(limbs str1, limbs str2) {              // return maximum of both strings.
    limbs max;
    //bool bothNeg = false;
    //bool isMax1 = false;
    //bool isMax2 = false;
    //if(str1[0] == '-' && str2[0] == '-') {
    //    bothNeg = true;
    //    str1.erase(0, 1);
    //    str2.erase(0, 1);
    //} else if(str1[0] == '-') {
    //    return trimmed(str2);
    //} else if(str2[0] == '-') {
    //    return trimmed(str1);
    //}
    //int str1_len = str1.size(), str2_len = str2.size();
    //if(str1_len == str2_len) {
    //    for(int i = 0; i < str1_len; ++i) {
    //        if(str1[i] != str2[i]) {
    //            if(str1[i] > str2[i]) {
    //                isMax1 = true;
    //            } else {
    //                isMax2 = true;
    //            }
    //            break;
    //        }
    //    }
    //    if(!(isMax2 ^ isMax1)) {
    //        isMax1 = true;
    //    }
    //} else if(str1_len > str2_len) {
    //    isMax1 = true;
    //} else {
    //    isMax2 = true;
    //}
    //if(bothNeg) {
    //    if(isMax1)
    //        return ("-" + trimmed(str2));
    //    else 
    //        return ("-" + trimmed(str1));
    //} else {
    //    if(isMax1)
    //        return trimmed(str1);
    //    else 
    //        return trimmed(str2);
    //}
    return max;
}

limbs bigint::minimum(limbs str1, limbs str2) {              // return minimum of both strings.
    limbs ans = maximum(str1, str2);
    if(ans == str1)
        return str2;
    return str1;
}

bool bigint::is_maximum(limbs str1, limbs str2) {             //checks if str1 >= str2, numerically
    if(str1 == maximum(str1, str2))
        return true;
    return false;
}

bool bigint::is_minimum(limbs str1, limbs str2) {             //checks if str1 <= str2, numerically
    if(str2 == maximum(str1, str2))
        return true;
    return false;
}

bool bigint::is_strictlyMaximum(limbs str1, limbs str2) {    //checks if str1 > str2, numerically
    if(str1 == str2)
        return false;
    if(str1 == maximum(str1, str2))
        return true;
    return false;
}

bool bigint::is_strictlyMinimum(limbs str1, limbs str2) {    //checks if str1 < str2, numerically
    if(str1 == str2)
        return false;
    if(str2 == maximum(str1, str2))
        return true;
    return false;
}

#pragma endregion

void bigint::trim(limbs& s) {                         // function to remove zeros
    size_t len = s.size(), i = len - 1;
    while (i > 0 && s[i] == 0)
        --i;
    s.resize(i + 1);
}

#pragma region Advanced mathematical functions



/*
        ______________________________________________________
                        Power Function
        ______________________________________________________
                      Algorithm Explanation

        It multiplies str1, str2 times and updates the str1 value
        
        Algorithmic Time Complexity : O(m), except multiplication time complexity
        Algorithmic Space Complexity : O(1)
        where n ans m are the lengths of respective strings(Integers) provided.
*/

limbs bigint::pow(limbs str1, limbs str2) {
    //if(str2 == "0") {
    //    return "1";
    //} else if(str1 == "0") {
    //    if(str2[0] == '-')
    //        return std::to_string((long long int)std::pow(0, -5));
    //    return "0";
    //} else if(str1[0] == '-' && str2[0] == '-') {
    //    if(str1 == "-1" && str2 == "-1") {
    //        return "-1";
    //    } else if(str1 == "-1") {
    //        if((((int)str2[str2.size()-1])-48) & 1) {                
    //            return "-1";
    //        } else {
    //            return "1";
    //        }
    //    } else {
    //        return "0";
    //    }
    //} else if(str1[0] == '-') {
    //    if((((int)str2[str2.size()-1])-48) & 1)
    //        return '-' + pow(str1.erase(0, 1), str2);
    //    return pow(str1.erase(0, 1), str2);
    //} else if(str2[0] == '-') {
    //    if(str1 == "1") {
    //        return str1;
    //    } else {
    //        return "0";
    //    }
    //} else {
    //    limbs init_str1 = str1;
    //    while(str2 != "1") {
    //        str1 = multiply(str1, init_str1);
    //        str2 = subtract(str2, "1");
    //    }
    //    return str1;
    //}
    return limbs();
}

/*
        ______________________________________________________
                        Sqrt Function
        ______________________________________________________
                      Algorithm Explanation

        Since sqrt of a string(s) has length floor(s/2), if s's length is even and 
        ceil(s/2) if s's length is odd.
        So it performs binary Search in that answer's range.
        
        Algorithmic Time Complexity : O(log(val. of s)), except divide time complexity
        Algorithmic Space Complexity : O(s/2);
*/

limbs bigint::sqrt(limbs s) {                 // returns sqrt(s), numerically
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

limbs bigint::log2(limbs s) {                 // returns log(s) to base of 2
    //if(s == "0") {
    //    // throw new exception(std::out_of_range("log(0) is undefined"));
    //    return std::to_string(std::log2(0));
    //}
    //if(s[0] == '-') {
    //    // throw new exception(std::out_of_range("log(negative) is not allowed"));
    //    return std::to_string(std::log2(-1));
    //}
    //limbs logVal = "-1";
    //while(s != "0") {
    //    add(logVal, logVal, "1");
    //    s = divide(s, "2");
    //}
    //return logVal;
    return limbs();
}

limbs bigint::log10(limbs s) {                // returns log(s) to base of 10
    //if(s == "0") {
    //    // throw new exception(std::out_of_range("log(0) is undefined"));
    //    return std::to_string(std::log2(0));
    //}
    //if(s[0] == '-') {
    //    // throw new exception(std::out_of_range("log(negative) is not allowed"));
    //    return std::to_string(std::log2(-1));
    //}
    //return std::to_string(s.size() - 1);
    return limbs();
}

limbs bigint::logwithbase(limbs val, limbs base) {
    limbs quotient, remainder;
    divide(quotient, remainder, log2(val), log2(base));
    return quotient;
}

limbs bigint::antilog2(limbs s) {
    //return pow("2", s);
    return limbs();
}

limbs bigint::antilog10(limbs s) {
    //return pow("10", s);
    return limbs();
}

void bigint::swap(limbs& str1, limbs& str2) {
    limbs s;
    s = str1;
    str1 = str2;
    str2 = s;
}

limbs bigint::reverse(limbs s) {          // reverses the string number, example: "7875" => "5787"
    //bool fl = false;
    //if(s[0] == '-') {
    //    s.erase(0, 1);
    //    fl = true;
    //}
    //unsigned long long int beg = 0, end = s.size() - 1;
    //char ch;
    //while(beg < end) {
    //    ch = s[beg];
    //    s[beg] = s[end];
    //    s[end] = ch;
    //    beg++;
    //    end--;
    //}
    //if(fl)
    //    s = '-' + s;
    //return s;
    return limbs();
}

limbs bigint::gcd(limbs str1, limbs str2) {
    if(is_strictlyMaximum(str2, str1))
        swap(str1, str2);
    limbs temp;
    while(is_strictlyMaximum(str2, limbs(1, 0))) {
        temp = mod(str1, str2);
        str1 = str2;
        str2 = temp;
    }
    return str1;
}

limbs bigint::lcm(limbs str1, limbs str2) {
    limbs quotient, remainder;
    divide(quotient, remainder, multiply(str1, str2), gcd(str1, str2));
    return quotient;
}

limbs bigint::fact(limbs s)
{
    limbs one{ 1 };
    if(s == limbs(1, 0))
        return limbs(1, 1);
    limbs ans = limbs(1, 1);
    while(s != limbs(1, 1)) {

        ans = multiply(ans, s);
        //multiply(ans, ans, s);
        subtract(s.begin(), s.cbegin(), s.cend(), one.cbegin(), one.cend());
    }
    return ans;
}

bigint bigint::factorial(size_t input)
{
    if (input <= 20) {
        llimb result = 1;
        for (limb i = 2; i <= input; ++i)
            result *= i;
        return bigint(result);
    }

    limbs result  { 1 },
          cache   { 1 };
    size_t progress = 3,
           iterator = 3,
           half     = input >> 1;

    size_t powOf2 = 0,
        totalPowOf2 = 0;

    llimb last = input;
    while (last >= 6) {
        last = last >> 1;
        powOf2 += 1;
    }
    progress = last >= 5 ? 5 : 3;
    totalPowOf2 = (powOf2 + 2) * (powOf2 + (last >= 4 ? 3 : 1)) / 2;

    while (progress <= half) {
        for (; iterator <= progress; iterator += 2) {
            cache = multiply(cache, bigint(iterator).str);
            totalPowOf2 += powOf2 * (powOf2 + 1) / 2;
        }
        result = multiply(result, cache);

        if (powOf2 <= 1)
            break;
        progress = progress << (powOf2 - 2);
        while (progress < input)
            progress += 1;
        progress = progress >> (powOf2 - 1);
        powOf2 -= 1;
    }

    for (; iterator <= input; iterator += 2)
        cache = multiply(cache, bigint(iterator).str);
    result = multiply(result, cache);

    shiftLeft(result, totalPowOf2);

    bigint ans; ans.str = result;
    return ans;
}

bool bigint::isPrime(limbs str) {
    if (!(str[0] & 1))
        return false;
    if (mod(str, { 3 }) == limbs(1, 0))
        return false;
    
    limbs two = { 2 }, four = { 4 },
                                sqrt = bigint::sqrt(str);
    // Now that 2 and 3 are out of the way, test all possible factors of the form 6n-1 and 6n+1
    for(limbs i = bigint(7).str; is_maximum(sqrt, i); add(i.begin(), i.cbegin(), i.cend(), two.cbegin(), two.cend())) {
        if(mod(str, i) == bigint(0).str)
            return false;
        add(i.begin(), i.cbegin(), i.cend(), four.cbegin(), four.cend());
        if (mod(str, i) == bigint(0).str)
            return false;
    }
    return true;
}

#pragma endregion

#endif
