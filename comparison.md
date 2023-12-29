# Comparison of big integers

The very first operators that big integers must be able to support are comparison operators: `==`, `!=`, `>`, `>=`, `<`, `<=` and `<=>` (since C++20).<br/>
All the operators above (and more) rely on a static private method to compare limbs of the big integers to each other. The method is declared with the following signature:
```c++
int compare(limbs const&, limbs const&);
```

The addition and subtraction operators rely on making comparisons when their operands are of opposite signs.

## Usage

```c++
using namespace bigint;
bigint a(123456789), b(987654321);
bool e  = (a == b),
     ne = (a != b),
     gt = (a >  b),
     ge = (a >= b),
     lt = (a <  b),
     le = (a <= b);

#if __cplusplus >= 202002L
std::strong_ordering ss = (a <=> b);
#endif
```

## Algorithm

All the functions behind these operators start with comparing the sign of the operands:
- If signs are unequal, a result can sometimes be returned immediately.
- Otherwise, limbs must be compared, using the private `compare` class method.<br/>Signs determine how the result of `compare(a.limbs, b.limbs)` must be interpreted for the result.

### Sign comparison

`a ... b` | `==` | `!=` | `>` | `>=` | `<` | `<=` | `<=>`
:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:
$\text{sign}(a) = \text{sign}(b) = 1$  | (1) | (2) | (3) | (4) | (5) | (6) | (7)
$\text{sign}(a) = \text{sign}(b) = -1$ | (1) | (2) | (6) | (5) | (4) | (3) | (8)
$\text{sign}(a) = 1 \text{ and } \text{sign}(b) = -1$ | (1) | (2) | ✔️ | ✔️ | ❌ | ❌ | ` std::strong_ordering::greater`
$\text{sign}(a) = -1 \text{ and } \text{sign}(b) = 1$ | (1) | (2) | ❌ | ❌ | ✔️ | ✔️ | ` std::strong_ordering::less`

The cases that could not be determined from signs alone, as listed above, are determined using the below formulas:
1. `compare(a.limbs, b.limbs) == 0`
2. `compare(a.limbs, b.limbs) != 0`
3. `compare(a.limbs, b.limbs) >  0`
4. `compare(a.limbs, b.limbs) >= 0`
5. `compare(a.limbs, b.limbs) <  0`
6. `compare(a.limbs, b.limbs) <= 0`
7. Execute:
    ```c++
    switch (compare(a.limbs, b.limbs)) {
        case -1: return std::strong_order::less;
        case 0 : return std::strong_ordering::equal;
        case 1 : return std:strong_ordering::greater;
    }
    ```
7. Execute:
    ```c++
    switch (compare(a.limbs, b.limbs)) {
        case -1: return std::strong_order::greater;
        case 0 : return std::strong_ordering::equal;
        case 1 : return std:strong_ordering::less;
    }
    ```

### Limbs comparison

The comparison of the limbs of `a` and `b` starts with a size comparison:
1. For `a.limbs` and for `b.limbs`, find the most significant, non-zero limb. Get an iterator, respectively `a_effective_end` and `b_effective_end` past that limb (they may be equal to their respective `cend()` iterator).
2. Calculate:
    - `a_size = std::distance(a.limbs.cbegin(), a_effective_end)`
    - `b_size = std::distance(b.limbs.cbegin(), b_effective_end)`[^1].
3. If `a_size > b_size`, return $1$.<br/>
If `a_size < b_size`, return $-1$.
4. From the most to the least significant limb:<br/>
   If `l_a > l_b`, return $1$.<br/>
   if `l_a < l_b`, return $-1$.
5. Reaching this point, all limbs from the 2 big integers have been compared as equal; return $0$;

Limb comparison is:
- $\text{O}(1)$ if the operands have a different size, i.e. iif $\exists p / a < 2^{32p} \leq b  \text{ or } b < 2^{32p} \leq a$.
- $\text{O}(\log(a))$ otherwise, $\log(a)$ being proportional to the lengths of the limbs.

[^1]: As limbs iterators are [LegacyRandomAccessIterator](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator), [`std::distance`](https://en.cppreference.com/w/cpp/iterator/distance) has constant complexity.
