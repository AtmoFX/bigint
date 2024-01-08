# Addition and subtraction

The first arithmetic operators to implement are `+` and `-`, along with assignment `+=` and `-=`, and pre/post-increment/decrement `++` and `--`.<br/>
Each of these operators allow to sum 2 big integers together or 1 big integer with a native C++ integer.

As the `bigint_t` type stores its sign separately from its limb, all the above operators need to [compare](./comparison.md) whether the signs of both operands are equal, then proceed with adding or subtracting the limbs to one another.<br/>
Just like the comparison operators rely on a static method called `compare`, the above operators rely on 2 static methods `add` and `subtract` to work specifically on the limbs. Conceptually speaking:
- `add(s, a, b);` is equivalent to doing $s \leftarrow |a| + |b|$.
- `subtract(d, a, b);` is equivalent to doing $d \leftarrow |a| - |b|$.

## Usage

```c++
using namespace bigint;
bigint a(123456789), b(987654321);
bigint s = a + b,
       d = a - b;

s+= 100;
--s;
d++;
```

## Algorithm

The algorithm always consists:
1. Check whether the operands $a$ and $b$ have the same sign.<br/>
If they do, proceed with the addition/subtraction of their limbs.<br/>
Otherwise, check the value of `compare(a.limbs, b.limbs)` to determine which method to call.<br/>
    _ | $a \times b \geq 0$<br/>$abs(a) \geq abs(b)$ | $a \times b \geq 0$<br/>$abs(a) < abs(b)$ | $a \times b < 0$<br/>$abs(a) \geq abs(b)$ | $a \times b < 0$<br/>$abs(a) < abs(b)$  
    ---|---|---|---|---
    $s \leftarrow a+b$ <td colspan="2">`s.sign = a.sign;`<br/>`add(s, a ,b);`</td> | `s.sign = a.sign;`<br/>`subtract(s, a, b);` | `s.sign = b.sign;`<br/>`subtract(s, b a);`</tr><tr><td>$d \leftarrow a - b$ | `s.sign = a.sign;`<br/>`subtract(d, a, b);` | `s.sign = b.sign;`<br/>`subtract(d, b a);` <td colspan="2">`s.sign = a.sign;`<br/>`add(d, a ,b);` </td>
2.  The algorithm for `add` and `subtract` are very similar in form, with $r, a, b$ representing respectively the output parameter and the limbs of the 2 operands:<br/>
    1. Resize $r$, filling it with 0 to:
        - For `add`, $1+\max(\text{size}(a), \text{size}(b))$.
        - For `subtract`, $\max(\text{size}(a), \text{size}(b))$.
    2. Initialize a variable, as $c \leftarrow 0$, to hold the carry/borrow at each step of the calculation.
    3. An iterator is initialized on the output parameter as well as both input parameters; all 3 iterators will be increased together.
    4. At every step, do perform an operation modulo $2^{32}$, with $c$ being updated with the carry/borrow:
        - For `add`: $2^{32} \times c + r_i \leftarrow a_i + b_i + c$.
        - For `subtract`: $2^{32} \times c + r_i \leftarrow a_i - b_i + c$.
    5. For `add`, set the last element of $r$ as $c$. 

## Technical note

The sum/subtraction of individual limbs with carry is done using 1 of 2 ways:
 - For compilers that provide the `intrin.h` header and the processor architecture is supported, then the intrinsic functions `_addcarry_u32` and `_subborrow_u32` are used to perform the sum/difference modulo $2^{32}$.
 - Otherwise, the operator is done in software.
