# Type definition and underlying math

Prior to writing any algorithm to manipulate big integers, we have to decide how they will be defined.<br/>
This section describes the different elements leading to the type definition for big integers.

## C++ type

### Container

There are 3 main requirements for a container to be able to define arbitrarily long integers:

1. It must be able to grow to any size, only limited by the amount of available memory.
1. It must preserve the least-to-most-significant or most-to-least-significant order of digit.<br/>
In C++, this translates into the container being a [SequenceContainer](https://en.cppreference.com/w/cpp/named_req/SequenceContainer).
1. Although this will only become clear when describing the algorithms behind the mathematical operations, it must support iterating over its element in reverse.
In C++, this translates into the container supporting [LegacyBidirectionalIterator](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)..

Excluding container adaptors and C-style arrays, here is a list of containers from the STL, available since C++11.

Type | Unlimited size | SequenceContainer | LegacyBidirectionalIterator
---|:---:|:---:|:---:
`std::basic_string` | ✔️ | ✔️ | ✔️
`std::deque` | ✔️ | ✔️ | ✔️
`std::list` | ✔️| ✔️ | ✔️
`std::vector` | ✔️ | ✔️ | ✔️
`std::array` | ❌ | ✔️ | ✔️ (partial)
`std::forward_list` | ✔️| ✔️ | ❌
`std::map` | ✔️| ❌ | ✔️
`std::multimap` | ✔️| ❌ | ✔️
`std::unordered_map` | ✔️| ❌ | ❌
`std::unordered_multimap` | ✔️| ❌ | ❌
`std::set` | ✔️| ❌ | ✔️
`std::multiset` | ✔️| ❌ | ✔️
`std::unordered_set` | ✔️| ❌ | ❌
`std::unordered_multiset` | ✔️| ❌ | ❌

Out of the 4 possible containers, the final choice is a matter of performance. In that regard, `std::basic_string` and `std:vector` come up similarly on top.

### Word

Libraries found across the Internet typically use one of 3 approaches:

1. Manipulate strings representing numbers, effectively implementing all the calculations in base $`10`$.
1. Work with native integer types, usually for half of the maximal integer size natively managed by the processor (i.e. 32-bit integers for modern architectures).
1. Work with base $`1,000,000,000`$, which mixes the previous 2 approaches in that it functions in a similar way to decimal numbers but packs digits into bigger groups to better use UC capabilities.
 
Base|Container|Memory usage|Pros|Cons
---|---|---|---|---
 $`10`$ | `std::string` | 41.5% ( $`= log_2(10)/8`$ )<br/>(3-4 bit used for every byte[^1]) | Most natural approach.<br/>Easiest to debug thanks to storing it as text.<br/>Fastest to print in base 10. | Waste of memory.<br/>Slow due to constant offset to character `'0'`. 
 $`10^9`$ | `std::vector<uint32_t>`[^2] | 93.4%<br/>(29-30 bits used for every 4 bytes) | Still a somewhat natural approach.<br/>Fast to print in base 10. | Harder to debug than working with `string`.
 $`2^{32}`$ | `std::vector<uint32_t>`[^2] | 100% | Best memory usage.<br/>Fastest calculation. | Hardest to debug.<br/>Slow to print in base 10.

[^1]: The logarithm is to represent the fact that the decimal base is less efficient than the hexadecimal base at storing big numbers with fewer digits. The hexadecimal base uses exactly 4 bits / digit, or 50% of each byte.
[^2]: `std::vector` can be substituted for `std::basic_string`

This approach chosen for this library is with 32-bit unsigned integers.

## Mathematic considerations

### Capacity requirements for mathematical operations

In any base $`b`$, it must be noted that any integer containing up to $`n`$ digits is at most equal to $`b^n - 1`$.<br/>
For the rest of this section, we will reason with **unsigned** integers with up to 32 bits, i.e. 32 digits when expressed in base $`2`$.

The results of the 4 basic operations can be stored in:

- Addition: $`n+1`$ digits.
- Subtraction: $`n`$ digits.
- Multiplication: $`2*n`$ digits.
- Division: $`1 + len(dividend) - len(divisor) \leq n`$ digits.
- Remainder: $`len(divisor) \leq n`$ digits.

Therefore, all the operations made on 32-bit integers fit in 64-bit integer types.

It is in fact possible to go a step further. With $`0 \leq a,b,c,d \leq 2^{32} - 1`$, $`a*b + c +d \leq 2^{64} - 1`$.<br/>
In other words, adding two 32-bit unsigned integers to the product of two 32-bit unsigned integers is guaranteed to fit in a 64-bit unsigned integer, and in fact, it fits **exactly** into 64 bits. 

**This will be important for the multiplication algorithm.**

Demonstration: $`\forall a,b,c,d, \text{   } 0 \leq a,b,c,d \leq 2^{32} - 1, \text{   } 0 \leq a*b+c+d \leq (2^{32} - 1) * (2^{32} - 1) + (2^{32} - 1) * 2 = (2^{32} - 1) * (2^{32} + 1) = 2^{64} - 1`$
