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
#include <iostream>
#include <stdexcept>
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
using limbs = std::vector<limb>;


#pragma warning(push)
#pragma warning(disable: 4710 4711)

namespace bigint {
    // Big Integer Class
    class bigint_t {
    public:
        limbs value;
#pragma warning( disable : 4820 )
        signed char sign;

        static void trim(limbs&);

        static int compare(
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );

        static int compare(const limbs& a, const limbs& b);


        static void add(
            limbs::iterator       d_begin,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        static void addInto(
            limbs::iterator       d_begin,
            limbs::const_iterator str_begin, limbs::const_iterator str_end
        );
        static void subtract(
            limbs::iterator       d_begin,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end);
        static void subtractFrom(
            limbs::iterator       from,
            limbs::const_iterator str_begin, limbs::const_iterator str_end
        );

        static void shiftLeft(limbs&, size_t);
        static void shiftRight(limbs&, size_t);

        static limbs multiply(const limbs& a, const limbs& b);
        static void mult(
            limbs::iterator       d_begin, limbs::iterator       d_end,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        static void mult_vanilla(
            limbs::iterator       d_begin, limbs::iterator       d_end,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        static void mult_karatsuba(
            limbs::iterator       d_begin, limbs::iterator       d_end,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        static void mult_toom3(
            limbs::iterator       d_begin, limbs::iterator       d_end,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        //static void multiply_schonhageStrassen(limbs&, const limbs& a, const limbs& b);


        static void divide(limbs& quotient, limbs& remainder, const limbs& a, const limbs& b);
        static void divideIn(limbs& value, limb divisor);
        static void div(
            limbs::iterator       d_begin, limbs&                remainder,
            limbs::const_iterator a_begin, limbs::const_iterator a_end,
            limbs::const_iterator b_begin, limbs::const_iterator b_end
        );
        static void div_vanilla(
            limbs::iterator       quotient, limbs& remainder,
            limbs::const_iterator dividend_begin, limbs::const_iterator dividend_end,
            limbs::const_iterator divisor_begin, limbs::const_iterator divisor_end);

        static void  mod(limbs&, limbs&, limbs, limbs);

        static bool  is_bigint(const std::string&);

        static void  swap(limbs&, limbs&);

        static void fromString(limbs& destination, signed char& sign, const std::string& source);

    public:
        bigint_t() :
            value(1, 0),
            sign(1)
        {
        }
        bigint_t(const std::string& source) :
            value(),
            sign(source[0] == '-' ? -1 : 1)
        {
            fromString(value, sign, source);
        }
        bigint_t(long long int n) :
            value(),
            sign(n >= 0 ? 1 : -1)
        {
            n = sign * n;
            n *= sign;
            do {
                value.push_back(static_cast<limb>(n));
                n >>= (8 * sizeof(limb));
            } while (n > 0);
        }
        bigint_t(int n) : bigint_t(static_cast<long long int>(n)) {}
        bigint_t(unsigned long long int n) :
            value(),
            sign(1)
        {
            do {
                value.push_back(static_cast<limb>(n));
                n >>= (8 * sizeof(limb));
            } while (n > 0);
        }

        bigint_t(const bigint_t& n) = default;

        void output(std::ostream& stream, unsigned short base = 10) const;
        void output_fast(std::ostream& stream, unsigned short base) const;

        std::string toString(unsigned short base = 10) const;

        friend std::ostream& operator << (std::ostream& stream, const bigint_t& n) {
            switch (stream.flags() & std::ios_base::basefield) {
                case stream.oct: n.output_fast(stream, 8) ; return stream;
                case stream.hex: n.output_fast(stream, 16); return stream;
                default:         n.output(stream, 10)     ; return stream;
            }
        }
        friend std::istream& operator >> (std::istream& stream, bigint_t& n) {
            std::string string_val;
            stream >> string_val;
            fromString(n.value, n.sign, string_val);
            return stream;
        }

        bigint_t operator + (bigint_t const& n) const {
            bigint_t ans;
            if (sign == n.sign) {
                ans.value.resize(std::max(value.size(), n.value.size()) + 1, 0);
                add(ans.value.begin(), value.cbegin(), value.cend(), n.value.cbegin(), n.value.cend());
            }
            else {
                auto comparison = compare(value, n.value);
                switch (comparison)
                {
                case 0: {
                    ans.sign = 1;
                    ans.value = { 0 };
                    return ans;
                }
                case -1: {
                    ans.value.resize(n.value.size(), 0);
                    subtract(ans.value.begin(), n.value.cbegin(), n.value.cend(), value.cbegin(), value.cend());
                    ans.sign = -sign;
                    break;
                }
                case 1: {
                    ans.value.resize(value.size(), 0);
                    subtract(ans.value.begin(), value.cbegin(), value.cend(), n.value.cbegin(), n.value.cend());
                    ans.sign = sign;
                    break;
                }
                }
            }
            trim(ans.value);
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
                if (n.value.size() >= value.size())
                    value.resize(n.value.size() + 1, 0);
                else if (value[value.size() - 1] == static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1))
                    value.resize(value.size() + 1, 0);
                addInto(value.begin(), n.value.cbegin(), n.value.end());
            }
            else {
                auto comparison = compare(value, n.value);
                switch (comparison)
                {
                case 0: {
                    sign = 1;
                    value = { 0 };
                    break;
                }
                case -1: {
                    value.resize(n.value.size(), 0);
                    subtract(value.begin(), n.value.cbegin(), n.value.cend(), value.cbegin(), value.cend());
                    sign = -sign;
                    break;
                }
                case 1: {
                    subtractFrom(value.begin(), n.value.cbegin(), n.value.cend());
                    break;
                }
                }
            }
            trim(value);
            return *this;
        }

        bigint_t operator - (bigint_t const& n) const
        {
            bigint_t ans;
            if (sign != n.sign) {
                ans.value.resize(std::max(value.size(), n.value.size()) + 1, 0);
                ans.sign = sign;
                add(ans.value.begin(), value.cbegin(), value.cend(), n.value.cbegin(), n.value.cend());
            }
            else {
                auto comparison = compare(value, n.value);
                switch (comparison)
                {
                case 0: {
                    ans.sign = 1;
                    ans.value = { 0 };
                    return ans;
                }
                case -1: {
                    ans.value.resize(n.value.size(), 0);
                    subtract(ans.value.begin(), n.value.cbegin(), n.value.cend(), value.cbegin(), value.cend());
                    ans.sign = -sign;
                    break;
                }
                case 1: {
                    ans.value.resize(value.size(), 0);
                    subtract(ans.value.begin(), value.cbegin(), value.cend(), n.value.cbegin(), n.value.cend());
                    ans.sign = sign;
                    break;
                }
                }
            }
            trim(ans.value);
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
                value.resize(std::max(value.size(), n.value.size()) + 1, 0);
                addInto(value.begin(), n.value.cbegin(), n.value.end());
            }
            else {
                auto comparison = compare(value, n.value);
                switch (comparison)
                {
                case 0: {
                    sign = 1;
                    value = { 0 };
                    break;
                }
                case -1: {
                    value.resize(n.value.size(), 0);
                    subtract(value.begin(), n.value.cbegin(), n.value.cend(), value.cbegin(), value.cend());
                    sign = -sign;
                    break;
                }
                case 1: {
                    subtractFrom(value.begin(), n.value.cbegin(), n.value.end());
                    break;
                }
                }
            }
            trim(value);
            return *this;
        }

        bigint_t operator * (bigint_t const& n) const {
            bigint_t ans;
            ans.sign = sign * n.sign;
            ans.value = multiply(value, n.value);
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
            value = multiply(value, n.value);
            return *this;
        }

        bigint_t operator / (bigint_t const& n) const
        {
            bigint_t ans;
            if (n.value.size() == 1) {
                if (n.value[0] == 0)
                    throw std::overflow_error("Division by zero");
                ans.value = value;
                divideIn(ans.value, n.value[0]);
            }
            else {
                limbs quotient, remainder;
                divide(quotient, remainder, value, n.value);
                ans.value = quotient;
            }
            return ans;
        }
        //friend bigint_t operator / (bigint_t const& n1, long long n2)
        //{
        //    return n1 / bigint_t(n2);
        //}
        friend bigint_t operator / (long long n1, bigint_t const& n2)
        {
            if (n2.value.size() > 2)
                return bigint_t(0LL);
            return bigint_t(n1) / n2;
        }
        bigint_t& operator /= (bigint_t const n)
        {
            limbs quotient, remainder;
            divide(quotient, remainder, value, n.value);
            value.swap(quotient);
            return *this;
        }
        friend bigint_t operator / (bigint_t n1, limb n2)
        {
            bigint_t quotient= n1;
            divideIn(quotient.value, n2);
            return quotient;
        }
        bigint_t& operator /= (limb n)
        {
            divideIn(value, n);
            return *this;
        }

        bigint_t operator % (bigint_t const& n) const {
            bigint_t ans;
            limbs quotient, remainder;
            mod(quotient, remainder, value, n.value);
            ans.value = remainder;
            return ans;
        }
        friend bigint_t operator % (bigint_t const& n1, long long n2) {
            return n1 % bigint_t(n2);
        }
        friend bigint_t operator % (long long n1, bigint_t const& n2) {
            if (n2.value.size() > 2)
                return n1;
            return bigint_t(n1) % n2;
        }
        bigint_t& operator %= (bigint_t const n) {
            limbs quotient, remainder;
            mod(quotient, remainder, value, n.value);
            std::swap(value, remainder);
            return *this;
        }

        bigint_t operator<<(size_t shiftCount)
        {
            bigint_t ans(*this);
            shiftLeft(ans.value, shiftCount);
            return ans;
        }
        bigint_t operator>>(size_t shiftCount)
        {
            bigint_t ans(*this);
            shiftRight(ans.value, shiftCount);
            return ans;
        }
        bigint_t& operator<<=(size_t shiftCount)
        {
            shiftLeft(value, shiftCount);
            return *this;
        }
        bigint_t& operator>>=(size_t shiftCount)
        {
            shiftRight(value, shiftCount);
            return *this;
        }

        bigint_t& operator ++ () {
            if (sign >= 0) {
                for (auto& s : value) {
                    if (s < (1ULL << (8 * sizeof(limb))) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                value.push_back(1);
            }
            else if (value.size() == 1 && value[0] == 1) {
                value[0] = 0;
                sign = 1;
            }
            else {
                for (auto& s : value) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                trim(value);
            }
            return *this;
        }
        bigint_t  operator ++ (int v) {
            if (v <= 0)
                v = 1;
            if (sign >= 0) {
                if (value[0] < (1ULL << (8 * sizeof(limb))) - v) {
                    value[0] += v;
                    return *this;
                }
                else if (v > 1) {
                    v -= static_cast<int>((1ULL << (8 * sizeof(limb))) - value[0]);
                    value[0] = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                for (auto& s : value) {
                    if (s < (1ULL << (8 * sizeof(limb))) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                value[0] += v;
                value.push_back(1);
            }
            else if (value.size() == 1 && value[0] == 1) {
                value[0] = 0;
                sign = 1;
            }
            else {
                for (auto& s : value) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                trim(value);
            }
            return *this;
        }
        bigint_t& operator -- () {
            if (sign < 0) {
                for (auto& s : value) {
                    if (s < (1ULL << (8 * sizeof(limb))) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                value.push_back(1);
            }
            else if (value.size() == 1 && value[0] == 0) {
                value[0] = 1;
                sign = -1;
            }
            else {
                for (auto& s : value) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                trim(value);
            }
            return *this;
            return *this;
        }
        bigint_t  operator -- (int v) {
            if (v <= 0)
                v = 1;
            if (sign < 0) {
                if (value[0] < (1ULL << (8 * sizeof(limb))) - v) {
                    value[0] += v;
                    return *this;
                }
                else if (v > 1) {
                    v -= static_cast<int>((1ULL << (8 * sizeof(limb))) - value[0]);
                    value[0] = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                for (auto& s : value) {
                    if (s < (1ULL << (8 * sizeof(limb))) - 1) {
                        s += 1;
                        return *this;
                    }
                    else
                        s = 0;
                }
                value[0] += v;
                value.push_back(1);
            }
            else if (value.size() == 1 && value[0] == 0) {
                value[0] = 1;
                sign = -1;
            }
            else {
                for (auto& s : value) {
                    if (s > 0) {
                        s -= 1;
                        return *this;
                    }
                    else
                        s = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
                }
                trim(value);
            }
            return *this;
        }

        bool operator ==(bigint_t const& n) const {
            return sign == n.sign && value == n.value;
        }
        bool operator !=(bigint_t const& n) const {
            return sign != n.sign || value != n.value;
        }
        bool operator > (bigint_t const& n) const {
            if (sign > n.sign)
                return true;
            else if (sign > 0)
                return compare(value, n.value) > 0;
            else
                return compare(value, n.value) < 0;
        }
        bool operator < (bigint_t const& n) const {
            if (sign < n.sign)
                return true;
            else if (sign > 0)
                return compare(value, n.value) < 0;
            else
                return compare(value, n.value) > 0;

        }
        bool operator >=(bigint_t const& n) const {
            if (sign > n.sign)
                return true;
            else if (sign > 0)
                return compare(value, n.value) >= 0;
            else
                return compare(value, n.value) <= 0;
        }
        bool operator <=(bigint_t const& n) const {
            if (sign < n.sign)
                return true;
            else if (sign > 0)
                return compare(value, n.value) <= 0;
            else
                return compare(value, n.value) >= 0;
        }
 
#if __cplusplus >= 202002L
        std::strong_ordering operator<=>(bigint_t const& n)
        {
            if (sign != n.sign)
                return sign <=> n.sign;
            else {
                if (auto comparison = compare(value, n.value); comparison == 0)
                    return std::strong_ordering::equal;
                else if (sign == comparison)
                    return std::strong_ordering::greater;
                else
                    return std::strong_ordering::less;
            }
        }
#endif

        friend bigint_t power    (const bigint_t&, size_t);
        friend bigint_t factorial(size_t n);
        friend bigint_t binomial (size_t n, size_t k);

    };

    /// @brief n^p, i.e. the product of \n with itself, with \n appearing \p times.
    /// @param[in] n Base.
    /// @param[in] p Exponent.
    /// @return n^p
    /// @exception May throw any exceptions thrown by \c std::allocator_traits<Allocator>::allocate(), such as \c std::bad_alloc.
    /// @exception \c std::length_error 
    bigint_t power(const bigint_t& n, size_t p);

    /// @brief n!, i.e. the product of all the positive integers less than or equal to \p n.
    /// @return n!
    /// @exception May throw any exceptions thrown by \c std::allocator_traits<Allocator>::allocate(), such as \c std::bad_alloc.
    /// @exception \c std::length_error 
    bigint_t factorial(size_t n);

    /// @brief Number of combinations of \p k elements chosen from a set of size \p n
    /// @param n Size of the set. 
    /// @param k Number of elements.
    bigint_t binomial(size_t n, size_t k);

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
                : vals{ T(0) }
            {
                for (size_t i = 0; i < order; ++i)
                    vals[i] = T(1);
                for (size_t r = 2; r < order; ++r)
                    vals[r * order - 1] = T(1);
            }
            FiboMatrix(std::array<T, order> values)
                : vals{ T(0LL) }
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
                    result[order - 1][c] = T(0);
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
                bigint_t::shiftLeft(newValue.value, 1);
                newValue -= *prevValue;
                result.push_back(newValue);
                std::advance(prevValue, 1);
                std::advance(curValue, 1);
            }
        }
    };



    /// @brief Fibonacci function or one of its generalization.
    /// @tparam order Order of the Fibonacci function, i.e. how many of the preceding numbers the current number is the sum of.
    /// @param[out] result Vector storing the consecutive Fibonacci numbers requested.
    /// @param[in] from Index of the first number to be returned (inclusive).
    /// @param[in] to Index of the last number to be returned (inclusive).
    /// @param[in] firstValues The forced first values of the generalized Fibonacci sequence. The number of values required is equal to \p order (default: 0, 0, ..., 0, 1, 1).
    template <unsigned int order>
#if __cplusplus >= 202002L    
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to, std::array<bigint_t, order> firstValues) requires lower_bound_value<order, 2>;
#else
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to, std::array<bigint_t, order> firstValues);
#endif

    template <unsigned int order = 2>
#if __cplusplus >= 202002L
    /// @overload
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to) requires lower_bound_value<order, 2>;
#else
    void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to);
#endif

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


#pragma region String input/output functions

void bigint::bigint_t::fromString(limbs& destination, signed char& sign, const std::string& source)
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
            throw std::invalid_argument("Invalid string was used to create a big integer.");

        llimb carry = static_cast<llimb>(*d) - '0';
        for (auto i = destination.begin(); i != destination.end(); std::advance(i, 1)) {
            llimb value = static_cast<llimb>(*i) * 10 + carry;
            *i = static_cast<limb>(value);
            carry = value >> (8 * sizeof(limb));
        }
        if (carry)
            destination.push_back(static_cast<limb>(carry));
    }
}

void bigint::bigint_t::output(std::ostream& stream, unsigned short base) const
{
    if (base < 2)
        throw std::runtime_error("Base is invalid");
    if (base > 64)
        throw std::runtime_error("Base is not supported");
    if ((base & (base - 1)) == 0) { //base is a power of 2
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
    for (auto d : Reverse<limbs>(value)) {
        llimb carry = 0;
        for (auto& s : result) {
            llimb value = (static_cast<llimb>(s) << (8 * sizeof(limb))) + carry;
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
    if (base == 0 || base > (8 * sizeof(limb)))
        throw std::runtime_error("Invalid base");

    unsigned short bitsPerDigit = 0;
    for (auto b = base; b > 1; b >>= 1) {
        if (b & 1)
            throw std::runtime_error("Invalid base");
        bitsPerDigit += 1;
    }
    auto        initialOffset = (value.size() * sizeof(limb) * 8) % bitsPerDigit,
        offsetPerLimb = sizeof(limb) % bitsPerDigit;

    bool          nonZero = false;
    unsigned char val     = 0;

    std::string   digits  = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    auto          l       = value.crbegin();
    if (initialOffset > 0) {
        val = *l >> (8 * sizeof(limb) - initialOffset);
        if (val > 0) {
            nonZero = true;
            stream << digits[val];
        }
    }
    for (; l != value.crend() && !nonZero; ++l) {
        std::string v;
        for (int s = 8 * sizeof(limb) - initialOffset - bitsPerDigit; s >= 0; s -= bitsPerDigit) {
            val = (*l >> s) & (base - 1);
            if (nonZero || val > 0) {
                nonZero = true;
                v += digits[val];
            }
        }
        stream << v;
    }

    if (nonZero) {
        if (l == value.crend())
            return;
        for (; l != value.crend(); ++l) {
            std::string v;
            for (int s = 8 * sizeof(limb) - bitsPerDigit; s >= 0; s -= bitsPerDigit) {
                auto val = (*l >> s) & (base - 1);
                v += digits[val];
            }
            stream << v;
        }
    }
    else
        stream << '0';
}

#pragma endregion


#pragma region Basic arithmetic functions

#pragma region Addition / Subtraction


void bigint::bigint_t::add(limbs::iterator sum,
    limbs::const_iterator a_begin, limbs::const_iterator a_end,
    limbs::const_iterator b_begin, limbs::const_iterator b_end)
{
    unsigned char carry = 0;
#ifndef _bigint_x86_x64_ARCH

    auto addChars = [&carry](limb v1, limb v2) -> limb
    {
        llimb sum = static_cast<llimb>(v1) + static_cast<llimb>(v2) + carry;
        if (sum >= (1ULL << (8 * sizeof(limb)))) {
            sum -= (1ULL << (8 * sizeof(limb)));
            carry = 1;
        }
        else
            carry = 0;
        return static_cast<limb>(sum);
    };
#endif
    limbs::const_iterator s1 = a_begin, s2 = b_begin;
    for (; s1 != a_end && s2 != b_end; ++s1, ++s2, std::advance(sum, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, *s1, *s2, &(*sum));
#else
        *sum = addChars(*s1, *s2);
#endif
        
    }

    for (; s1 != a_end; ++s1, std::advance(sum, 1)) {
#ifdef _bigint_x86_x64_ARCH
        carry = _addcarry_u32(carry, *s1, 0, &(*sum));
#else
        *sum = addChars(*s1, 0);
#endif
    }
    for (; s2 < b_end; ++s2, std::advance(sum, 1)) {
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
        if (sum >= (1ULL << (8 * sizeof(limb)))) {
            sum -= (1ULL << (8 * sizeof(limb)));
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
        while (*into == static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1)) {
            *into = 0;
            ++into;
        }
        *into += 1;
    }
}


void bigint::bigint_t::subtract(limbs::iterator delta,
                      limbs::const_iterator a_begin, limbs::const_iterator a_end,
                      limbs::const_iterator b_begin, limbs::const_iterator b_end)
{
    unsigned char borrow = 0;
#ifndef _bigint_x86_x64_ARCH
    auto subLimbs = [&carry] (limb v1, limb v2) -> limb
    {
        int64_t diff = static_cast<int64_t>(v1) - static_cast<int64_t>(v2) + carry;
        if (diff < 0) {
            diff += (1LL << (8 * sizeof(limb)));
            carry = -1;
        }
        else
            carry = 0;
        return static_cast<limb>(diff);
    };
#endif
    auto s1 = a_begin, s2 = b_begin;
    for (; s1 != a_end && s2 != b_end; ++s1, ++s2, std::advance(delta, 1)) {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, *s1, *s2, &(*delta));
#else
        *from = subLimbs(*s1, *s2);
#endif
    }

    for (; s1 != a_end; ++s1, std::advance(delta, 1)) {
#ifdef _bigint_x86_x64_ARCH
        borrow = _subborrow_u32(borrow, *s1, 0, &(*delta));
#else
        *delta = subLimbs(*s1, 0);
#endif
    }
        
    for (; s2 != b_end; ++s2, std::advance(delta, 1))
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
    auto l = std::distance(str_begin, str_end);

    unsigned char borrow = 0;
#ifdef _bigint_x86_x64_ARCH
    for (auto s = str_begin; s != str_end; ++s, std::advance(from, 1)) {
        l = std::distance(s, str_end);
        borrow = _subborrow_u32(borrow, *from, *s, &(*from));
    }
#else
    auto subLimbs = [&borrow](limb v1, limb v2) -> limb
        {
            int64_t diff = static_cast<int64_t>(v1) - static_cast<int64_t>(v2) - borrow;
            if (diff < 0) {
                diff += (1LL << (8 * sizeof(limb)));
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
            *from = static_cast<limb>((1ULL << (8 * sizeof(limb))) - 1);
            ++from;
        }
        *from -= borrow;
    }
}


#pragma endregion


#pragma region Multiplication

limbs bigint::bigint_t::multiply(const limbs& a, const limbs& b)
{
    //trim(const_cast<limbs&>(a)); trim(const_cast<limbs&>(b));
    size_t a_len = a.size(), b_len = b.size();
    limbs product; product.resize(a_len + b_len, 0);
    mult(product.begin(), product.end(), a.begin(), a.end(), b.begin(), b.end());   
    trim(product);
    return product;
}

void bigint::bigint_t::mult(
    limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator a_begin, limbs::const_iterator a_end,
    limbs::const_iterator b_begin, limbs::const_iterator b_end
) {
    constexpr size_t karatsubaThreshold = 128,
                     toom3Threshold  = 10000;

    ptrdiff_t a_len = std::distance(a_begin, a_end),
              b_len = std::distance(b_begin, b_end),
              calc_len = std::min(a_len, b_len);
    

    if (calc_len >= toom3Threshold && a_len * 6 >= b_len * 5 && a_len * 5 <= b_len * 6)
        mult_toom3(product, product_end, a_begin, a_end, b_begin, b_end);
    else if (calc_len >= karatsubaThreshold)
        mult_karatsuba(product, product_end, a_begin, a_end, b_begin, b_end);
    else
        mult_vanilla(product, product_end, a_begin, a_end, b_begin, b_end);
}


void bigint::bigint_t::mult_vanilla(
    limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator a_begin, limbs::const_iterator a_end,
    limbs::const_iterator b_begin, limbs::const_iterator b_end
) {
    limb carry = 0;
    auto multChars = [&carry](auto v1, auto v2, auto e) -> limb
    {
        llimb product = static_cast<llimb>(v1) * static_cast<llimb>(v2) + carry + e;
        carry = static_cast<limb>(product >> (8 * sizeof(limb)));
        return static_cast<limb>(product);
    };
    //auto multChars = [&carry](auto v1, auto v2, auto& e) -> void
    //    {
    //        llimb product = static_cast<llimb>(v1) * static_cast<llimb>(v2) + carry + e;
    //        carry = static_cast<limb>(product >> (8 * sizeof(limb)));
    //        z = static_cast<limb>(product);
    //    };

    for (auto s1 = a_begin; s1 != a_end; ++s1, std::advance(product, 1)) {
        carry = 0;
        auto op1 = *s1;
        auto prod_iter = product;
        for (auto s2 = b_begin; s2 != b_end; ++s2, std::advance(prod_iter, 1)) {
            auto& existing = *prod_iter;
            existing = multChars(op1, *s2, existing);
        }
        *prod_iter += carry;
    }
}


void bigint::bigint_t::mult_karatsuba(
    limbs::iterator       product, limbs::iterator       product_end,
    limbs::const_iterator a_begin, limbs::const_iterator a_end,
    limbs::const_iterator b_begin, limbs::const_iterator b_end
) {
    ptrdiff_t len1 = std::distance(a_begin, a_end),
              len2 = std::distance(b_begin, b_end),
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
    auto a_mid = a_begin, b_mid = b_begin;
    std::advance(a_mid, splitO); std::advance(b_mid, splitO);
    auto product_mid = product, product_midSum = product, product_mid2 = product;
    std::advance(product_mid, splitP);    std::advance(product_midSum, splitO);    std::advance(product_mid2, splitO);

    // product(2^0) <- b*d
    mult(product, product_mid, a_begin, a_mid, b_begin, b_mid);
    // product(2^2k) <- a*c
    mult(product_mid, product_end, a_mid, a_end, b_mid, b_end);

    limbs a_b, c_d, intermediate, intermediateSum;
    a_b.resize(static_cast<size_t>(len1 - splitO + 1), 0); c_d.resize(static_cast<size_t>(len2 - splitO + 1), 0); intermediate.resize(static_cast<size_t>(len1 + len2 - splitP + 2), 0); intermediateSum.resize(static_cast<size_t>(std::max(len1 + len2 - splitP, splitP) + 1), 0);

    add(a_b.begin(), a_begin, a_mid, a_mid, a_end);
    add(c_d.begin(), b_begin, b_mid, b_mid, b_end);
    mult(intermediate.begin(), intermediate.end(), a_b.cbegin(), a_b.cend(), c_d.cbegin(), c_d.cend());
    
    //a*c + b*d
    add(intermediateSum.begin(), product, product_mid, product_mid, product_end);

    addInto(product_midSum, intermediate.cbegin(), intermediate.cend());
    subtractFrom(product_midSum, intermediateSum.cbegin(), intermediateSum.cend());

    //print("result", product, product_end);
}

#include <chrono>
//See algorithm at https://github.com/AtmoFX/bigint/blob/master/documentation/multiplication.md#Toom-Cook
// Notation = lowercase variable = strings of limbs; uppercase variable = polynomial
void bigint::bigint_t::mult_toom3(
    limbs::iterator       product_begin, limbs::iterator       product_end,
    limbs::const_iterator a_begin      , limbs::const_iterator a_end,
    limbs::const_iterator b_begin      , limbs::const_iterator b_end
)
{
    size_t size_a = std::distance(a_begin, a_end),
           size_b = std::distance(b_begin, b_end),
           l      = std::min(size_a, size_b),
           l_third = (l + 2) / 3 // = RoundUp(l/3, 0)
        ;
        
    limbs::const_iterator
        a1_begin = a_begin  + l_third,
        a2_begin = a1_begin + l_third,
        b1_begin = b_begin  + l_third,
        b2_begin = b1_begin + l_third;
    limbs::iterator
        p0_begin = product_begin,
        p1_begin = p0_begin + l_third,
        p2_begin = p1_begin + l_third,
        p3_begin = p2_begin + l_third,
        p4_begin = p3_begin + l_third;

    using std::chrono::steady_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    bigint_t a0, a1, a2, b0, b1, b2;
    a0.value.resize(l_third); a1.value.resize(l_third); a2.value.resize(size_a - 2 * l_third);
    b0.value.resize(l_third); b1.value.resize(l_third); b2.value.resize(size_b - 2 * l_third);
    std::copy(a_begin, a1_begin, a0.value.begin()); std::copy(a1_begin, a2_begin, a1.value.begin()); std::copy(a2_begin, a_end, a2.value.begin());
    std::copy(b_begin, b1_begin, b0.value.begin()); std::copy(b1_begin, b2_begin, b1.value.begin()); std::copy(b2_begin, b_end, b2.value.begin());

    bigint_t va_1 = a2 + a0 - a1, vb_1 = b2 + b0 - b1,
        va1 = a2 + a1 + a0, vb1 = b2 + b1 + b0,
        va2 = (a2 << 2) + (a1 << 1) + a0, vb2 = (b2 << 2) + (b1 << 1) + b0;

    //P(-1), P(1) and P(2)
    bigint_t P_1 = va_1 * vb_1,
             P1 = va1 * vb1,
             P2 = va2 * vb2;

    // p0 = P(0) and p4 = P(infinity)
    bigint_t p0  = a0 * b0,
             p4  = a2 * b2;

    bigint_t p0_plus_p4 = p0 + p4;
    bigint_t p2 = ((P_1 + P1) >> 1) - p0_plus_p4,
             p3 = (p0 - 14 * p4 + P2 - ((p2 + P1) << 1)) / 6,
             p1 = P1 - (p0_plus_p4 + p2 + p3);

    addInto(p0_begin, p0.value.cbegin(), p0.value.cend());
    addInto(p1_begin, p1.value.cbegin(), p1.value.cend());
    addInto(p2_begin, p2.value.cbegin(), p2.value.cend());
    addInto(p3_begin, p3.value.cbegin(), p3.value.cend());
    addInto(p4_begin, p4.value.cbegin(), p4.value.cend());
}

#pragma endregion

#pragma region Division/mod functions

void bigint::bigint_t::divide(limbs& quotient, limbs& remainder, const limbs& a, const limbs& b)
{
    size_t a_len = a.size(),
           b_len = b.size();

    if (b_len == 0 || std::all_of(b.cbegin(), b.cend(), [](limb l) { return l == 0; }))
        throw std::runtime_error("Division by 0");

    if (b_len == 1) {
        //auto dividend = std::make_reverse_iterator(a.end());
        //auto q = std::make_reverse_iterator(quotient); std::advance(q, -a_len);
        //llimb r = 0;
        //for (; dividend != std::make_reverse_iterator(a.begin()) && q != std::make_reverse_iterator(quotient); ++dividend, ++q) {
        //    *q = static_cast<limb>((b * r + *dividend) / b[0]);
        //    r = r * b + *dividend - *q * *b[0];
        //}
        //*remainder = static_cast<limb>(r);
    }

    if (b_len > a_len || compare(a, b) < 0) {
        quotient = { 0 };
        remainder = b;
        return;
    }

    unsigned long requiredShift = 0;
    auto top_divisor_limb = std::make_reverse_iterator(b.end());
    for (; top_divisor_limb != std::make_reverse_iterator(b.begin()); ++top_divisor_limb) {
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
        size_t quotient_len = a_len - b_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(b_len + 1, 0);

        div(quotient.begin(), remainder, a.cbegin(), a.cend(), b.cbegin(), b.cend());
    }
    else {
        limbs shiftedDividend(a), shiftedDivisor(b);
        shiftLeft(shiftedDividend, requiredShift);
        shiftLeft(shiftedDivisor, requiredShift);
        a_len = shiftedDividend.size(), b_len = shiftedDivisor.size();
        size_t quotient_len = a_len - b_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(b_len + 1, 0);

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
    bigint_t dividend, divisor;
    dividend.value.resize(std::distance(dividend_begin, dividend_end), 0);
    divisor.value.resize(std::distance(divisor_begin, divisor_end), 0);
    std::copy(dividend_begin, dividend_end, dividend.value.begin());
    std::copy(divisor_begin, divisor_end, divisor.value.begin());

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
        shiftLeft(progress, static_cast<size_t>((dividend_len - divisor_len) * (8 * sizeof(limb)) - 1));

    auto d = std::make_reverse_iterator(dividend_end);
    std::advance(d, divisor_len - 1);
    std::copy(d.base(), dividend_end, remainder.begin());

    llimb significant_div_digit = *(divisor_end - 1);
    auto quotient_end = quotient;

    for (; d != std::make_reverse_iterator(dividend_begin); ++d) {
        shift(remainder.begin(), remainder.end(), *d);
        llimb value = (static_cast<llimb>(remainder[remainder.size() - 1]) << (8 * sizeof(limb))) + remainder[remainder.size() - 2];
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
                carry = (value >> (8 * sizeof(limb))) + c;
            }
            while (carry > 0) {
                unsigned short c = (*r >= static_cast<limb>(carry)) ? 0 : 1;
                *r -= static_cast<limb>(carry);
                ++r;
                carry = c;
            }
            while (compare(remainder.begin(), remainder.end(), divisor_begin, divisor_end) >= 0) {
                //{
                //    bigint_t r; r.value = remainder;
                //    print("remainder", r);
                //}
                result += 1;
                subtract(remainder.begin(), remainder.begin(), remainder.end(), divisor_begin, divisor_end);
            }
            //{
            //    bigint_t r; r.value = remainder;
            //    print("remainder", r);
            //}
        }
        ++quotient_end;
        shift(quotient, quotient_end, static_cast<limb>(result));
        shiftRight(progress, (8 * sizeof(limb)));
    }
}

void bigint::bigint_t::mod(limbs& quotient, limbs& remainder, limbs a, limbs b)
{
    size_t a_len = a.size(),
        b_len = b.size();

    if (b_len == 0 || std::all_of(b.cbegin(), b.cend(), [](limb l) { return l == 0; }))
        throw std::runtime_error("Division by 0");

    if (b_len == 1) {
        //auto dividend = std::make_reverse_iterator(a.end());
        //auto q = std::make_reverse_iterator(quotient); std::advance(q, -a_len);
        //llimb r = 0;
        //for (; dividend != std::make_reverse_iterator(a.begin()) && q != std::make_reverse_iterator(quotient); ++dividend, ++q) {
        //    *q = static_cast<limb>((b * r + *dividend) / b[0]);
        //    r = r * b + *dividend - *q * *b[0];
        //}
        //*remainder = static_cast<limb>(r);
    }

    if (b_len > a_len || compare(a, b) < 0) {
        quotient = { 0 };
        remainder = b;
        return;
    }

    unsigned long requiredShift = 0;
    auto top_divisor_limb = std::make_reverse_iterator(b.end());
    for (; top_divisor_limb != std::make_reverse_iterator(b.begin()); ++top_divisor_limb) {
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
        size_t quotient_len = a_len - b_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(b_len + 1, 0);

        div(quotient.begin(), remainder, a.cbegin(), a.cend(), b.cbegin(), b.cend());
    }
    else {
        limbs shiftedDividend(a), shiftedDivisor(b);
        shiftLeft(shiftedDividend, requiredShift);
        shiftLeft(shiftedDivisor, requiredShift);
        a_len = shiftedDividend.size(), b_len = shiftedDivisor.size();
        size_t quotient_len = a_len - b_len + 1;
        quotient.resize(quotient_len, 0); remainder.resize(b_len + 1, 0);

        div(quotient.begin(), remainder, shiftedDividend.cbegin(), shiftedDividend.cend(), shiftedDivisor.cbegin(), shiftedDivisor.cend());

        shiftRight(remainder, requiredShift);
    }
    trim(remainder);
}


void bigint::bigint_t::divideIn(limbs& value, limb divisor)
{
    auto v1 = value.rbegin(), v2 = v1;
    llimb val = *v1;
    //val <<= 32;
    for (std::advance(v1, 1); v1 != value.rend(); ++v1, ++v2) {
        *v2 = static_cast<limb>(val / divisor);
        val = val % divisor;
        val <<= 32;
        val += static_cast<llimb>(*v1);
        //val <<= 32;
    }
    *v2 = val / divisor;
}

#pragma endregion

void bigint::bigint_t::shiftLeft(limbs& value, size_t shiftCount)
{
    size_t subIntValue = shiftCount & 31;

    limb carry = 0;
    for (auto s = value.begin(); s != value.end(); std::advance(s, 1)) {
        llimb a = (static_cast<llimb>(*s) << subIntValue) + carry;
        *s = static_cast<limb>(a);
        carry = a >> (8 * sizeof(limb));
    }

    if (carry)
        value.push_back(carry);
    if (shiftCount >= (8 * sizeof(limb)))
        value.insert(value.begin(), shiftCount >> 5, 0);
}

void bigint::bigint_t::shiftRight(limbs& value, size_t shiftCount)
{
    if ((shiftCount >> 5) >= value.size()) {
        value = { 0 };
        return;
    }

    value.erase(value.begin(), value.begin() + (shiftCount >> 5));

    size_t subIntValue = shiftCount & 31;

    unsigned int carry = 0;
    for (auto s = value.rbegin(); s != value.rend(); std::advance(s, 1)) {
        auto shiftValue = *s & ((1 << subIntValue) - 1);
        *s = (*s >> subIntValue) + (carry << ((8 * sizeof(limb)) - subIntValue));
        carry = shiftValue;
    }

    trim(value);
}


#pragma endregion

#pragma region Comparison functions

int bigint::bigint_t::compare(limbs::const_iterator a_begin, limbs::const_iterator a_end,
    limbs::const_iterator b_begin, limbs::const_iterator b_end)
{
    int result = 0;
    auto s1 = a_begin, s2 = b_begin;
    for (; s1 != a_end && s2 != b_end; ++s1, ++s2) {
        if (*s1 > *s2)
            result = 1;
        else if (*s1 < *s2)
            result = -1;
    }

    for (; s1 != a_end; ++s1) {
        if (*s1 != 0)
            return 1;
    }
    for (; s2 != b_end; ++s2) {
        if (*s2 != 0)
            return -1;
    }

    return result;
}

int bigint::bigint_t::compare(const limbs& a, const limbs& b)
{
    return compare(a.cbegin(), a.cend(), b.cbegin(), b.cend());
}

#pragma endregion

void bigint::bigint_t::trim(limbs& s) {
    size_t len = s.size(), i = len - 1;
    while (i > 0 && s[i] == 0)
        --i;
    s.resize(i + 1);
}

#pragma region Advanced artithmetic functions

bigint::bigint_t bigint::power(const bigint::bigint_t& n, size_t p)
{
    bigint::bigint_t result = 1, cache;
    result.sign = (p & 1) ? n.sign : 1; cache.value = n.value;

    while (p > 0) {
        if (p & 1)
            result *= cache;
        cache *= cache;
        p >>= 1;
    }
    return result;
}

void bigint::bigint_t::swap(limbs& a, limbs& b) {
    std::swap(a, b);
}

#pragma endregion


#pragma region Advanced mathematical functions

bigint::bigint_t bigint::factorial(size_t n)
{
    // For n <= 20, factorial(n) fits in a 64-bit unsigned integer -> returned values are hardcoded.
    if (n <= 20) {
        switch (n) {
            case 0:
            case 1:  return bigint_t(1ULL);
            case 2:  return bigint_t(2ULL);
            case 3:  return bigint_t(6ULL);
            case 4:  return bigint_t(24ULL);
            case 5:  return bigint_t(120ULL);
            case 6:  return bigint_t(720ULL);
            case 7:  return bigint_t(5040ULL);
            case 8:  return bigint_t(40320ULL);
            case 9:  return bigint_t(362880ULL);
            case 10: return bigint_t(3628800ULL);
            case 11: return bigint_t(39916800ULL);
            case 12: return bigint_t(479001600ULL);
            case 13: return bigint_t(6227020800ULL);
            case 14: return bigint_t(87178291200ULL);
            case 15: return bigint_t(1307674368000ULL);
            case 16: return bigint_t(20922789888000ULL);
            case 17: return bigint_t(355687428096000ULL);
            case 18: return bigint_t(6402373705728000ULL);
            case 19: return bigint_t(121645100408832000ULL);
            case 20: return bigint_t(2432902008176640000ULL);
        }
    }

    limbs result{ 1 }, cache{ 1 };
    size_t progress = 3, iterator = 3, half = n >> 1, powOf2 = 0, totalPowOf2 = 0;

    llimb last = n;
    while (last >= 6) {
        last = last >> 1;
        powOf2 += 1;
    }
    progress = last >= 5ULL ? 5ULL : 3ULL;
    totalPowOf2 = (powOf2 + 2) * (powOf2 + (last >= 4 ? 3 : 1)) / 2;

    while (progress <= half) {
#pragma warning(suppress: 4293)
        limbs newCache = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 1 >> (8 * sizeof(limb) - 1)) }; //Double bit shift for 32/64-bit compatibility
        totalPowOf2 += powOf2 * (powOf2 + 1) / 2;
        iterator += 2;
        for (; iterator <= progress; iterator += 2) {
#pragma warning(suppress: 4293)
            limbs val = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 1 >> (8 * sizeof(limb) - 1)) };
            newCache = bigint_t::multiply(newCache, val);
            totalPowOf2 += powOf2 * (powOf2 + 1) / 2;
        }
        cache = bigint_t::multiply(cache, newCache);
        result = bigint_t::multiply(result, cache);

        if (powOf2 <= 1)
            break;
        progress = progress << (powOf2 - 2);
        while (progress < n)
            progress += 1;
        progress = progress >> (powOf2 - 1);
        powOf2 -= 1;
    }
    {
#pragma warning(suppress: 4293)
        limbs newCache = { static_cast<limb>(iterator), static_cast<limb>(iterator >> 1 >> (8 * sizeof(limb) - 1)) };
        iterator += 2;
        for (; iterator <= n; iterator += 2)
#pragma warning(suppress: 4293)
            newCache = bigint_t::multiply(newCache, { static_cast<limb>(iterator), static_cast<limb>(iterator >> 1 >> (8 * sizeof(limb) - 1)) });

        cache = bigint_t::multiply(cache, newCache);
    }
    result = bigint_t::multiply(result, cache);

    bigint_t::shiftLeft(result, totalPowOf2);

    bigint_t ans; ans.value = result;
    return ans;
}

template <unsigned int order>
void bigint::fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to, std::array<bigint_t, order> firstValues)
#if __cplusplus >= 202002L    
requires lower_bound_value<order, 2>
#endif
{
    result.reserve(to - from + 1);
    if (from <= 8 * order * order) { // Special case: from is too small for calls to fibonacci_matrix to make sense
        std::vector<bigint_t> intermediate;
        intermediate.reserve(from + order + 1);
        bigint_t nextFibo;
        for (auto v : firstValues) {
            intermediate.push_back(v);
            nextFibo += v;
        }
        intermediate.push_back(nextFibo);
        if (from + order >= to) {
            fibonacci_consecutive<order>(intermediate, to - order - 1);
            for (auto i = from - 1; i < to; ++i)
                result.push_back(intermediate[i]);
            return;
        }
        fibonacci_consecutive<order>(intermediate, from - 1);
        auto i = intermediate.cend();
        i -= (order + 1);
        for (; i != intermediate.cend(); ++i)
            result.push_back(*i);
    }
    else { // General case: from is big enough to use the matrix exponantiation algorithm
        if (result.capacity() <= order)
            result.reserve(order + 1);
        FiboMatrix<bigint_t, order> initial_value_matrix(firstValues);
        fibonacci_matrix<order>(result, from, initial_value_matrix);
        if (to - from < order)
            result.resize(to - from + 1);
    }
    if (to - from > order)
        fibonacci_consecutive<order>(result, to - from - order);

}

template <unsigned int order>
void bigint::fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to)
#if __cplusplus >= 202002L
requires lower_bound_value<order, 2>
#endif
{
    std::array<bigint_t, order> standardFirstValues{ bigint_t(0LL) };
    standardFirstValues[order - 1] = standardFirstValues[order - 2] = bigint_t(1LL);
    return fibonacci(result, from, to, standardFirstValues);
}


bigint::bigint_t bigint::binomial(size_t n, size_t k)
{
    if (k > n || n == 0)
        return bigint::bigint_t();
    size_t nk = n - k;

    if (k > nk)
        std::swap(k, nk);
    bigint::bigint_t denom1 = 1ULL, denom2 = 1ULL;

}
#pragma endregion

#pragma warning(pop)
#endif
