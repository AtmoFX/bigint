# Fibonacci sequence

The Fibonacci sequence is a well-known sequence of integers in which each number is the sum of the two preceding ones.

This library provides generalizations of the Fibonacci sequence, in the form of:
- Sequences of higher order, i.e. in which each number is the sum of the $`k`$, instead of two, preceding ones.<br/>
These sequences are usually referred to as Tribonacci, Tetranacci, Pentanacci, Hexanacci, ...
- Sequences taking a different starting point.

This makes it an infinite family of sequences.

## Function signature

The fibonacci function is defined as:

```c++
template <unsigned int order = 2>
void fibonacci(std::vector<bigint_t>& result, unsigned int from, unsigned int to);
```

For algorithmic complexity reasons, it returns consecutive Fibonacci numbers located at indices between `from` and `to`.<br/>
We note $\text{fibonacci{<}}k\text{{>}}(n)$: $f^k_n$ and $\text{fibonacci}(n) = \text{fibonacci{<}}2\text{{>}}(n)$ simplified as $f_n$. 

## Naive implementation

The naive implementation of a Fibonacci function, for a single number makes use of the recursive nature of the sequence:

```c++
unsigned long long fibonacci(short n)
{
  if (index <= 1)
    return index;
  return fibonacci(n - 1) + fibonacci(n - 2);
}
```

This is of course highly inefficient: because each call creates 2 recursive branches, the complexity is $\text{O}(2^n)$. With this approach:
- $f_{40}$ takes about 1sec. to calculate on modern PC (only the order of magnitude matters, not the exact duration).
- $f_{93}$, which is the largest value that fits in the `unsigned long long` type, is impossible to compute in practice.

## Alternative implementations

Several approaches allow the execution time to be greatly cut, among which we can list the following.

### Memoization

Caching the successive Fibonacci numbers can cut both branches of the naive algorithm. With a cache, there is no need going all the way back to $f_1$, which the naive approach does $\approx 2^n$ times.

```c++
#include <vector>
std::vector<uint64_t> cache = { 0, 1 };
uint64_t fibonacci(unsigned int n) {
    if (n < cache.size())
        return cache[n];

    uint64_t value = fibonacci(n - 1) + fibonacci(n - 2);
    cache.push_back(value);
    return value;
}
```

This definitely works but it takes a lot of memory to calculate thousands, if not millions of terms.<br/>
With an associative container type and a little more work to save values on disk, it is possible to create an executable that can resume from the last time it was executed.

### Calculation at compile-time

With the template function below, it is possible to have the compiler save Fibonacci numbers in an array. This is an alternative to the above memoization approach, with the difference that the cache is compiled into the executable.

```c++
template<std::size_t N>
constexpr auto compile_fibonacci_values()
{
    static_assert(N > 2);
    std::array<std::uint64_t, N> values{ 1ul,1ul };
    for (std::size_t n{ 2ul }; n < N; ++n)
        values[n] = (values[n - 1] + values[n - 2]);

    return values;
}

constexpr auto fibonacci(std::size_t n)
{
    constexpr auto fibonacci_values = compile_fibonacci_values<93>();
    return fibonacci_values[n - 1];
}
```

The above version uses 64-bit integers and as such is limited to $f_{93}$, although it could be extended to cover more Fibonacci numbers using big integers.

Even if it was changed to use big integers, this approach still is limited in that the sequence it generates only goes up to an arbitrary index decided at compile time.
By the time we reach $f_{1{,}000{,}000}$, the executable it creates becomes huge and the Fibonacci numbers that follow stay out of reach: the computation time required to get $f_{1{,}000{,}093}$ is the same as $f_{93}$ with the naive approach.<br/>
Finally, it does not allow a program to save its state and resume from there on its next execution.

## The library's approach

As we determined above, the memory required to save any significant number of Fibonacci numbers greatly limits how useful the above 2 approaches are. 
It also makes the implementation of the generalizations to the Fibonacci sequence impossible.

2 algorithms compete to achieve get fibonacci numbers of a higher index in the sequence, and with enough flexibility the calculation can be generalized:
- Iterative algorithm:
With just 2 variables, it is possible to completely get rid of the recursion from the naive algorithm.<br/>
With this approach, $f_n$ can be obtained in $\approx n$ steps[^1].
- Matrix exponantiation: Fibonacci numbers can also be calculated by multiplying square matrices together; the size of the matrix is the same as the order ($`2`$ for the Fibonacci sequence).<br/>
Thanks to [exponantiation by squaring](https://en.wikipedia.org/wiki/Exponentiation_by_squaring), that we have already used for the [`power` function](./power.md) and is now applied to matrices, $f_n$ can be obtained in $\approx \log(n)$ steps[^1].<br/>

[^1]: Not to be mixed with algorithmic complexity, since each "step" has its own complexity.

So, it is hardly a competition if both algorithms have this difference in complexity. Well, not so fast...
- The downside of the matrix exponantiation method is that it calculates independent values. To have all the values between $f_n$ to $f_m$, it is necessary to restart from 1.<br/>
It is easy to understand that the calculation of $`n`$ values of something requires at least an $\text{O}(n)$ algorithm. The iterative algorithm is perfectly suited for this.
- Even if the matrix exponantiation algorithm could be modified to calculate consecutive values with the same algorithmic complexity as the iterative algorithm, it would still be slower.<br/>
Indeed, the iterative algorithm, knowing $f_{n-2}$ and $f_{n-1}$, can output $f_n$ in a single addition. This is faster than anything achievable with matrices.

### End-to-end calculation

Getting the results consists in linking the matrix exponantiation algorithm to the iterative algorithm;. To get consecutive Fibonacci numbers from index $m$ to index $n$:

1. Apply the matrix exponantiation algorithm to get values from index $m$ to $m + k$. This is done by calculating $\mathscr{F}^k_{m+1}$.
2. If $m + k \geq n$, discard the unnecessary values, if any, and stop.<br/>
Otherwise, apply the iterative algorithm, using the Fibonacci numbers obtained at step 1 to kick it off.

In practice, the matrix exponantiation algorithm is called only if the index of the first Fibonacci number that must be returned is high enough; the threshold is closely related to the size of the fibonacci matrix ($\text{order} \times \text{order}$) and the number of iterations required to get passed the iterative algorithm, it is currently set at $8 \times \text{order}^2$. For smaller values, the benefit-to-cost of the exponantiation is in favor of the iterative function.

### Iterations

The principle of this loop is to calculate consecutive values of the Fibonacci sequence, by performing sum of the previous, already calculated numbers.<br/>
The corresponding function is called last but as it is much simpler, we will describe it first.

#### Inputs

The algorithm has a number of parameters:
- The first and last indices of the Fibonacci number to calculate, respectively $\text{from}$ and $\text{to}$.
- The order $k$ of the sequence.
- The first $f^k_1, f^k_2, \dotsc, f^k_k$ numbers of the sequence.

#### Algorithm

1. Initialize $f^k_0 = 0$ and $f^k_1, f^k_2, \dotsc, f^k_k$ from the provided initial values.
2. Add all $f^k_i / \text{from} \leq i \leq k$ to the output, if any.
3. Let $i = k + 1$.
4. Do $f^k_i \leftarrow f^k_{i - k} + f^k_{i - k + 1} + \dotsc + f^k_{i - 1}$.
5. If $i \geq \text{from}$, add $f_i$ to the function's output.
6. If $i = \text{to}$, return the output.<br/>Otherwise, do $i \leftarrow 1 + i$.
7. If $p \leq \text{to}$, go back to step 4.

#### Optimization for higher orders

At higher orders of the sequence, the $k$ terms in the sum $f^k_i \leftarrow f^k_{i - k} + f^k_{i - k + 1} + \dotsc + f^k_{i - 1}$ can be optimized:

$$
\begin{align*}
\forall i, f^k_i = \sum_{n = 1}^k f^k_{i - n} = & \text{  } \sum_{n = 1}^k f^k_{i - n} + f^k_{i-k-1} - f^k_{i-k-1} \\
= & \text{  } f^k_{i-1} +  \sum_{n = 2}^{k +1} f^k_{i - n} - f^k_{i-k-1} \\
= & \text{  } 2 \times f^k_{i-1} - f^k_{i-k-1} 
\end{align*}
$$

At the cost of knowing $k+1$ consecutive elements of the Fibonacci series of order $k$ (1 more than would otherwise be required), it is possible to calculate all the next elements with only 2 basic operations; starting at $k=4$, this calculation is faster than simply applying the definition formula of the sequence.

This creates a constraint on the matrix exponantiation algorithm: we must make sure it generates $k+1$ consecutive values we can use, otherwise the end-to-end calculation cannot be performed in the way we have described.<br/>
Hopefully, as we are about to see, this is exactly what the matrix exponantiation algorithm generates.

### Matrix exponantiation

The principle here is to perform exponantiation by squaring to a $k \times k$ matrix:

$$
\mathscr{F}^2_n = 
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}^n =
\begin{pmatrix}
f_{n+1} & f_n \\
f_n & f_{n-1}
\end{pmatrix}
$$

Again, exponantiation by squaring allows to get the result  faster than $\text{O}(n)$, the downside being we cannot get more than $k+1$ values.

#### Inputs

The algorithm's inputs are:
- The target index $n$
- The order $k$ of the sequence.
- The first $f^k_1, f^k_2, \dotsc, f^k_k$ numbers of the sequence.

#### Algorithm

1. Initialize variables $F = R = \mathscr{F}^k_1$.
2. If $n \bmod 2 = 1$, do $R \leftarrow R \times F$.
3. Do $M \leftarrow M^2$.<br/>This is the step that gives the name, exponantiation by squaring, to the algorithm.
4. Do $n \leftarrow \lfloor n/2 \rfloor$.
5. If $n > 1$ go back to step 2.
6. Do $R \leftarrow R \times F$.
7. Return the Fibonacci numbers from the matrix.<br/>
The bottom-right value ($f^k_n$); the values in the left column, from bottom to top, are $f^k_{n+1}, f^k_{n+2}, \dotsc, f^k_{n+k}$.<br/>This makes it $k+1$ values, exactly as required by the iterative function.

#### Matrices for higher order sequences

At order $k$, the matrix used for exponantiation is made of zeroes except for the ones placed:
- On the first row.
- On the cells directly under the diagonal.

$$
\mathscr{F}^k_n =
\begin{pmatrix}
1 & 1 & 1 & \cdots & 1 & 1 \\
1 & 0 & 0 & \cdots & 0 & 0 \\
0 & 1 & 0 & \cdots & 0 & 0 \\
0 & 0 & 1 & \cdots & 0 & 0 \\
\vdots & \vdots & \vdots & \ddots & \vdots & \vdots \\
0 & 0 & 0 & \cdots & 1 & 0
\end{pmatrix}^n =
\begin{pmatrix}
f^k_{n+k-1} & \cdots & f^k_{n+k-2} \\
f^k_{n+k-2} & \cdots & f^k_{n+k-3} \\
\vdots & \ddots & \vdots \\
f^k_n & \cdots & f^k_{n-1}
\end{pmatrix}
$$

#### Optimizations   of the matrix multiplication

When multiplying two $k \times k$ matrices, each item of the result is obtained by doing $k$ products and $k-1$ additions to sum them together. 
Setting aside the complexity of the multiplication operation, this makes it a $\text{O}(k^3)$ operation[^2]. Hopefully, the matrices used in this algorihtm are not any matrices: as they are used to generate a sequence following a pattern, they have a pattern of their own that is the direct consequence of how $\mathscr{F}^k_1$ is structured.

[^2]: Better algorithms exist but require bigger values for $k$ than we typically expect here.

This pattern allows to remove a substantial number of multiplications:

1. $k - 1$ values appear twice:<br/>
Values from the first column, row $r \geq 2$ are fibonacci numbers that are repeated on the last column of the matrix, at row $r - 1$.
There is no need, during exponantiation, to do all the multiplications more than once per number.<br/>
Additionally, there is no need to reserve memory to store these numbers twice. The matrices will be represented as a C++ array of size $k^2 - (k - 1)$ embedded into a class whose responsibility to is to turn the 1D-array into a 2D-matrix:<br/>
Below is but a fraction of the definition of that class, to illustrate how the matrix-to-array index conversion (`M[r][c] -> A[i]`).
```c++
template<typename T, unsigned int order>
class FiboMatrix {
    static_assert(order >= 2);
public:
    T* operator[](size_t r) {
        return &vals[r * (order - 1)];
    }
private:
    std::array<T, order * order - order + 1> vals;
};
```
The above class implements matrix-to-array index correspondance as:

$$
\begin{pmatrix}
0 & 1 & 2 & 3 & \cdots & \color{red}k-1 \\
k-1 & k & k+1 & k+2 & \cdots & \color{red}2(k-1) \\
2(k-1) & 2k-1 & 2k & 2k+1 & \cdots & \color{red}3(k-1) \\
\vdots & \vdots & \vdots & \vdots & \ddots & \vdots \\
(k-2)(k-1) & (k-2)(k-1)+1 & (k-2)(k-1)+2 & (k-2)(k-1)+3 & \cdots & \color{red}(k-1)^2 \\
(k-1)^2 & (k-1)^2+1 & (k-1)^2+2 & (k-1)^2+3 & \cdots & k^2-k
\end{pmatrix}
$$


2. The other optimization comes from the fact that:<br/>
$\forall k,n, \forall r,c \lt k, \mathscr{F}^k_n[r,c] = \mathscr{F}^k_n[r,k] + \mathscr{F}^k_n[r+1,c +1]$<br/>
This means that except for the last row and the last column, every item of $\mathscr{F}^k_n$ can be calculated with a single addition, which is orders of magnitude faster than processing $k$ multiplications (+ $k-1$ additions).

Illustration with:<br/>
$
\mathscr{F}^5_{10} =
\begin{pmatrix}
464 & 448 & 417 & 356 & 236 \\
236 & 228 & 212 & 181 & 120 \\
 120 & 116 & 108 & 92 & 61 \\
61 & 59 & 55 & 47 & 31 \\
31 & 30 & 28 & 24 & 16
\end{pmatrix}
$

$
\mathscr{F}^{10}_{12} =
\begin{pmatrix}
2045 & 2043 & 2039 & 2031 & 2015 & 1983 & 1919 & 1791 & 1535 & 1023 \\
1023 & 1022 & 1020 & 1016 & 1008 & 992 & 960 & 896 & 768 & 512 \\
512 & 511 & 510 & 508 & 504 & 496 & 480 & 448 & 384 & 256 \\
256 & 256 & 255 & 254 & 252 & 248 & 240 & 224 & 192 & 128 \\
128 & 128 & 128 & 127 & 126 & 124 & 120 & 112 & 96 & 64 \\
64 & 64 & 64 & 64 & 63 & 62 & 60 & 56 & 48 & 32 \\
32 & 32 & 32 & 32 & 32 & 31 & 30 & 28 & 24 & 16 \\
16 & 16 & 16 & 16 & 16 & 16 & 15 & 14 & 12 & 8 \\
8 & 8 & 8 & 8 & 8 & 8 & 8 & 7 & 6 & 4 \\
4 & 4 & 4 & 4 & 4 & 4 & 4 & 4 & 3 & 2
\end{pmatrix}
$

```math
\begin{align*}
\mathscr{F}_{10}^5[2,2]    & = & 228  & = & 120 + 108   & = \;\; & \mathscr{F}_{10}^5[2,5]     \;\;\;\; & + \;\;\;\; \mathscr{F}_{10}^5[3,3] \\
\mathscr{F}_{12}^{10}[1,4] & = & 2031 & = & 1023 + 1008 & = \;\; & \mathscr{F}_{12}^{10}[1,10] \;\;\;\; & + \;\;\;\; \mathscr{F}_{12}^{10}[2,5] \\
\mathscr{F}_{12}^{10}[3,8] & = & 448  & = & 256 + 192   & = \;\; & \mathscr{F}_{12}^{10}[3,10] \;\;\;\; & + \;\;\;\; \mathscr{F}_{12}^{10}[4,9]
\end{align*}
```


The combination of the above 2 rules allows to perform the multiplication by working our way up: 

1. Calculate the bottom row using normal matrix multiplication.<br/>
As per point 1. above, $\mathscr{F}^k_n[k, 1] = \mathscr{F}^k_n[k-1, k]$.
2. Let variable $r = k-1, c=k-1$.
3. Do  $\mathscr{F}^k_n[r, c] = \mathscr{F}^k_n[r, k]+\mathscr{F}^k_n[r+1, c+1]$.
4. If $r=c=1$, stop.<br/>
If $c>1$ do $c \leftarrow c - 1$.
Otherwise, do $r \leftarrow r-1, c \leftarrow k-1$
5. Go back to step 3.

The result is that for a $k \times k$ matrix multiplication, the bottom $k$ elements each require $k$ multiplications to be obtained (and $k-1$) additions). The $(k-1)^2$ remaining elements need 1 addition each to be obtained. The resulting computational complexity therefore is $\text{O}(k^2)$, or $\text{O}(\text{M}(n) \times k^2)$ when including the complexity of multiplications; this is true for all $k$, i.e. not simply as an asymptotic behavior (when $k$ is large enough).<br/>
By exploiting the specific structure of the matrices, the algorithm we obtain is faster than any known general-case matrix multiplication algorithm and at least as fast as the theoretical limit[^3].

[^3]: Whether matrix multiplication can be done with $\text{O}(n^2)$ complexity is an open question in mathematics.



### Defining a different starting point

Different sequences can be generated depending on the first elements in the series. Among the common generalized sequences:
- The Fibonacci sequence starts with $f_0 = 0, f_1 = 1$.
- The Lucas sequence starts with $g_0 = 2, g_1 = 1$.

Thanks to overriding the starting point of a sequence, it is possible to save the state of a calculation to resume it later. As an example:
- With $g_1 = f_{100} = 354{,}224{,}848{,}179{,}261{,}915{,}075 \text{ and  } g_2 = f_{101} = 573{,}147{,}844{,}013{,}817{,}084{,}101$
- The relationship that links the Fibonacci sequence with that generalization is: $\forall n, g_n = f_{n+99}$.<br/>
  For instance, $f_{199} = g_{100} = 173{,}402{,}521{,}172{,}797{,}813{,}159{,}685{,}037{,}284{,}371{,}942{,}044{,}301$ 

#### Algorithm

When calculating sequences with a different starting point, the iterative algorithm and the matrix exponantiation algorithm can both be used. In both cases, the function needs the first $k$ elements, noted $e_1, e_2, \dotsc, e_k$, to be able to compute the rest of the sequence:
- For the iterative algorithm, the function simply has to substitute the first elements defined by default by those passed to it.
- For the matrix exponantiation algorithm, a matrix of $k \times k$ elements needs to be defined from the $k$ input parameters.
Interestingly, the same properties used to speed up the matrix multiplication can also be used to reconstruct the right matrix $\mathscr{M_1^k}$:
1. For every row of the matrix, do: $\mathscr{M}_1^k[r,1] = e_{k-r+1}$.<br/>
This operation automatically fills all but the last element of the last column of $\mathscr{M}_1^k$.
2. Do $\mathscr{M}_1^k[k,k] = e_0 = e_k - (e_1 + e_2+\dotsc+e{k-1})$
3. Initialize variables $r = 1, c= k-1$.
4. Do $\mathscr{M}_1^k[r,c] \leftarrow \mathscr{M}_1^k[r,k] + \mathscr{M}_1^k[r+1,c+1]$
5. If $r = c = 2$, go to step 6.<br/>
If $r \leq c$, do $r \leftarrow r + 1$.<br/>
Otherwise, do $r \leftarrow 1, c \leftarrow c-1$.
6. Do $r \leftarrow 3, c \leftarrow 2$.
7. Do $\mathscr{M}_1^k[r,c] \leftarrow \mathscr{M}_1^k[r-1,c-1] - \mathscr{M}_1^k[r,k]$.
8. If $r=k \text{ and } c = k-2$, stop.<br/>
If $r < k$, do $r \leftarrow r + 1$.
Otherwise, do $c \leftarrow c + 1, r \leftarrow c+1$.
9. Go back to step 7.

<br/>

Illustration with $k=4, e_1=4, e_2=8, e_3=1, e_4=3$
1. Initialization:<br/>

$$
\begin{align*}
e_0 = 3 - 1 - 8- 4 = -10 \\
\mathscr{M}_1^4 \leftarrow
\begin{pmatrix}
3 & \color{red}? & \color{red}? & 1 \\
1 & \color{red}? & \color{red}? & 8 \\
8 & \color{red}? & \color{red}? & 4 \\
4 & \color{red}? & \color{red}? & -10
\end{pmatrix}
\end{align*}
$$

2. First loop, on column 3:<br/>

$$
\mathscr{M}_1^4 \leftarrow
\begin{pmatrix}
3 & \color{red}? & 9 & 1 \\
1 & \color{red}? & 12 & 8 \\
8 & \color{red}? & -6 & 4 \\
4 & \color{red}? & \color{red}? & -10
\end{pmatrix}
$$

3. First loop, on column 2:<br/>

$$
\mathscr{M}_1^4 \leftarrow
\begin{pmatrix}
3 & 13 & 9 & 1 \\
1 & 2 & 12 & 8 \\
8 & \color{red}? & -6 & 4 \\
4 & \color{red}? & \color{red}? & -10
\end{pmatrix}
$$

4. Second loop, on column 2:<br/>

$$
\mathscr{M}_1^4 \leftarrow
\begin{pmatrix}
3 & 13 & 9 & 1 \\
1 & 2 & 12 & 8 \\
8 & -7 & -6 & 4 \\
4 & 4 & \color{red}? & -10
\end{pmatrix}
$$

5. Second loop, on column 3:<br/>

$$
\mathscr{M}_1^4 \leftarrow
\begin{pmatrix}
3 & 13 & 9 & 1 \\
1 & 2 & 12 & 8 \\
8 & -7 & -6 & 4 \\
4 & 4 & -11 & -10
\end{pmatrix}
$$

#### Stop-and-start example

The code below prints every 100 Fibonacci numbers between indices 100k and 200k. It calculates all the values before anything is printed, so it needs about 1.25GB to store them all in memory.

```c++
constexpr unsigned int minIndex = 100000, maxIndex = 200000, reportEvery = 100;
std::vector<bigint::bigint_t> fiboResults;
bigint::fibonacci(fiboResults, minIndex, maxIndex);

for (unsigned int i = 0; i <= maxIndex - minIndex; i += reportEvery)
    std::cout << "Fibonacci(" << minIndex + i << "):\n" << fiboResults[i].toString() << '\n';
```

An alternative to the above approach is to force the starting point to actually skip the calculation of the unnecessary numbers. The code is more verbose but requires less than 2MB of memory, as never more than 3 Fibonacci numbers  are in memory (+ everything the `fibonacci` function needs to work).<br/>



```c++
constexpr unsigned int minIndex = 100000, maxIndex = 200000, reportEvery = 100;
std::vector<bigint::bigint_t> fiboResults;
bigint::fibonacci(fiboResults, minIndex, minIndex + 2);

std::cout << "Fibonacci(" << minIndex << "):\n" << fiboResults[0].toString() << '\n';
for (unsigned int i = minIndex + reportEvery; i <= maxIndex; i += reportEvery) {
    std::array<bigint::bigint_t, 2> restartFrom;
    std::swap(restartFrom[0], fiboResults[1]); std::swap(restartFrom[1], fiboResults[2]);
    fiboResults.clear();
    bigint::fibonacci(fiboResults, reportEvery, reportEvery + 2, restartFrom);
    std::cout << "Fibonacci(" << i << "):\n" << fiboResults[0].toString() << '\n';
}
```

Note that in the above code, while we could use `fiboResult[0]` and `fiboResult[1]` to fill the `restartFrom` array, that would be very slightly sub-optimal: as the matrix exponantiation algorithm generates 1 more value than the order (i.e. 3 values), we may as well use them all rather than discarding it.
