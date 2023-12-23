# Factorial

## Function signature

The factorial function is defined as:
```c++
bigint_t factorial(size_t n);
```

The in-parameter `n` is limited to type `size_t` because:
 - Calculating a factorial anything near $`10^9!`$ is very long in practice, let alone values closer to $`10^{19}!`$ which is still far from what `size_t` can store on 64-bit architectures ($`\approx 1.845 \times 10^{19}`$).
 - Since `size_t` is linked to how much memory a system holds, the space required to store the result of factorial bigger than `size_t` integers exceeds the capabilities. In practice, that limit is hit long before reaching the maximum value a `size_t` can express.

## Naive implementation

The naive implementation for factorial calculation is along the lines of:

```c++
bigint::bigint_t bigint::factorial(size_t n)
{
  bigint::bigint_t result = 1;
  for (size_t i = 2; i <= n; ++i)
    result *= i;
  return result;
}
```

This approach suffers from several downsides:
 - It requires `n - 1` multiplications (from `2` to `n`).<br/>
For instance, we would want $`3`$, $`6`$, $`12`$, $`24`$ (i.e. $`3`$'s multiplied by a power of $`2`$) to be processed as $`64 \times (3^2)^2`$: This requires 2 multiplications (The multiplication by $`64`$ is done through bit shifting) instead of 3. 
 - The operands passed in the successive multiplications are very different in size.<br/>
 Having one of the operand consiting of 1 or 2 limbs prevents from using the more efficient multiplication algorithms.
 - As was mentioned above, we can make use of bit shifting operations when multiplying even numbers.<br/>
In fact, it is possible to account for all the $`2`$'s encountered between $`1`$ and $`n`$ with a single shift.

## The library's approach

### Principle

Instead of the naive implementation, the implemented algorithm works "backward", with only odd numbers ($`3`$, $`5`$, $`7`$, $`9`$ ...).<br/>
For the sake of clarity, we will illustrate each step with $`n = 20`$. The table below represents the factors; when used, they get crossed out.

|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

### Algorithm

1. Initialize the result variable $`\text{result} \leftarrow 1`$.
2. Determine $`p`$ so that $`2^p \leq \frac{n}{3}`$. It is possible that $`2^p \leq n / 5`$.<br/>
Example: with $`n = 20`$: $`2^3 \gt \frac{20}{3}`$ and $`2^2 \leq \frac{20}{3}`$. This means $`p = 2`$.
3. Initialize a variable $`\text{product} \leftarrow 3`$.
4. Find the highest odd integer $`i`$ that verifies $`2^p \gt \frac{20}{i}`$. With $`n = 20`$ and $`p = 2`$, that number is $`5`$ ($`2^2 \leq \frac{20}{5}`$).<br/>
For every **additional** odd number $`i`$ that verifies the above inequality, do $`\text{product} \leftarrow \text{product} * i`$.
5. Do $`\text{result} \leftarrow \text{result} \times \text{product}`$

|1|2|3|4|5|6|7|8|9|10|11|<strike>12</strike>|13|14|15|16|17|18|19|<strike>20</strike>|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

6. If $`p > 0`$, do $`p \leftarrow p - 1`$ then go back to **step 4**.
  - At $`p = 1`$, the numbers to add to product are $`7`$ and $`9`$ and we consume all the factors that are multiples of $`2`$ without being multiples of $`4`$.<br/>
    This includes $`6`$ and $`10`$ which were already covered by $`\text{product}`$ for no computing cost at all.<br/>
    First: $`\text{product} \leftarrow \text{product} \times 7 \times 9`$<br/>
    Then $`\text{result} \leftarrow \text{result} \times \text{product}`$
    
|1|2|3|4|5|<strike>6</strike>|7|8|9|<strike>10</strike>|11|<strike>12</strike>|13|<strike>14</strike>|15|16|17|<strike>18</strike>|19|<strike>20</strike>|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

  - At $`p = 0`$, the numbers are $`11`$, $`13`$, $`15`$, $`17`$ and $`19`$ and we consume all the factors that are not multiples of $`2`$.<br/>
    Like above, $`3`$, $`5`$, $`7`$, $`9`$ were already included in $`\text{product}`$, thus are included again.
    First: $`\text{product} \leftarrow \text{product} \times 11 \times 13 \times 15 \times 17 \times 19`$<br/>
    Then $`\text{result} \leftarrow \text{result} \times \text{product}`$

|1|2|<strike>3</strike>|4|<strike>5</strike>|<strike>6</strike>|<strike>7</strike>|8|<strike>9</strike>|<strike>10</strike>|<strike>11</strike>|<strike>12</strike>|<strike>13</strike>|<strike>14</strike>|<strike>15</strike>|16|<strike>17</strike>|<strike>18</strike>|<strike>19</strike>|<strike>20</strike>|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

7. All that remains is shift $`\text{result}`$ by the correct amount to account for all the even numbers.<br/>
Doing so at the last step saves some execution time as all the previous multiplications did not have to browse through the additional limbs that shift creates.

|1|<strike>2</strike>|<strike>3</strike>|<strike>4</strike>|<strike>5</strike>|<strike>6</strike>|<strike>7</strike>|<strike>8</strike>|<strike>9</strike>|<strike>10</strike>|<strike>11</strike>|<strike>12</strike>|<strike>13</strike>|<strike>14</strike>|<strike>15</strike>|<strike>16</strike>|<strike>17</strike>|<strike>18</strike>|<strike>19</strike>|<strike>20</strike>|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

### Time savings

Compared to the naive approach and for big enough factorials, the number of multiplications is only half the naive approach (the complexity is the same) and it makes better use of the optimized multiplication algorithms, as both $`\text{product}`$ and $`\text{result}`$ grow.
