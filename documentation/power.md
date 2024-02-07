# Power

## Function signature

The power function is defined as:

```c++
bigint_t power(const bigint_t& n, size_t p);
```

This calculates $n^p$, with $p$ being a positive integer.

## Native implementation

The native implementation for power calculation is:

```c++
bigint::bigint_t power(const bigint::bigint_t& n, size_t p)
{
    bigint::bigint_t result = 1ULL;
    for (size_t i = 0; i < p; ++i)
        result *= n;
    return result;
}
```

This approach suffers from 2 downsides:
 - It requires `p` multiplications.
 - The multiplications (`result * n`) are all done against the same `n`.<br/>
 The multiplication algorithms get increasingly more efficient with the number of limbs of each operand (`(a * b) * (c * d)` is faster to calculate than `((a * b) * c ) * d`). $n$ remaining the same, its number of limbs never grows either and efficient multiplication algorithm can never be leveraged.

 ## The library's approach

 ### Principle

 This library uses [exponantiation by squaring](https://simple.wikipedia.org/wiki/Exponentiation_by_squaring), which addresses the 2 downsides listed above.<br/>Exponantiation by squaring is an **important** algorithm, which is also used in the calculation of [Fibonacci numbers](fibonacci.md).

 Instead of simply multiplying $n$ by itself $\text{O}(p)$ times, we decompose $p$ into its separate digits $p_k, p_{k-1}, \dotsc, p_0$, when written in binary form.

 $$
 n^p = \prod_{0 \leq i \leq k, p_i = 1} \big( p_i \times n^{(2^i)} \big)
 $$

The algorithm makes use of the fact that $n^{2^i} = (n^{2^{i-1}})^2$ to get each consecutive $n^{2^i}$ in only 1 multiplication, i.e. by multiplying $n^{2^{i-1}}$ obtained at the previous loop to itself.

Illustration for $3^{19}$:
1. Decompose the exposant into its binary digits: $19 = 2^0 + 2^1 + 2^4$
2. We start with $i = 0$, i.e. $3^{2^0} = 3$.
3. For $i = 1$, we calculate $3^{2^1} = 3 \times 3 = 9$.
4. For $i = 2$, we calculate $3^{2^2} = 9 \times 9 = 81$.<br/>
This is not going to be used in the final product but we need it for the next step of the calculation.
5. For $i = 3$, we calculate $3^{2^3} = 81 \times 81 = 6{,}561$.<br/>
Same as above, this is only useful to get to the next step of the calculation.
6. For $i = 4$, we calculate $3^{2^4} = 6{,}561 \times 6{,}561 = 43{,}046{,}721$.
7. We have now covered all the digits of $19$.<br/>
Since $19 =2^0 + 2^1 + 2^4$, we can calculate $3^{19} = 3 \times 9 \times 43{,}046{,}721 = 1{,}162{,}261{,}467$.

The algorithm uses $\lfloor log_2(p) \rfloor$ squaring operations and at most $\lfloor log_2(p) \rfloor$ multiplications for the final product (it depends how many binary digits of $p$ are $1$).In the above case, the result was obtained with 6 multiplications, instead of 18 for the naive case.<br/>
In other words, setting aside the complexity of the multiplication operation, this algorithm has a complexity of $O(log(p))$.

### Algorithm

1. Initialize $\text{result} \leftarrow 1, \text{cache} \leftarrow n$.
2. If $p = 0$, stop and return $\text{result}$.
3. If $p \bmod 2 = 1$, do $\text{result} \leftarrow \text{result} \times \text{cache}$.
4. Do $\text{cache} \leftarrow \text{cache} \times \text{cache}$.
5. Do $p \leftarrow \lfloor p / 2 \rfloor$.
6. Go back to step 2.
