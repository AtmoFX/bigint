# bigint

C++ header for big integers, with explanations.

To get started, download the [single header file](https://github.com/source/bigint.h), include it in your code, and compile however you would.

## A summary of the project

**Do you need thousands of digits? Millions? More? This library got you covered!<br/>
Are you after documentation but cannot be bothered looking up for research papers? Browse the resources here.**

There are already many big integer libraries out there:

- GMP (The GNU Multiple Precision Arithmetic Library) is by far the most well-known, and not only for integers.
With more than 30 years since its first release, it has become the reference to achieve the best possible performance.<br/>
It is also a very complex piece of work written in C and Assembler, hard to include in modern C++ projects (especially under Windows) and definitely not the easiest way to get an introduction to the underlying mathematics and algorithms.
- Other libraries have a different offer: much more simple but for most of them, painfully slow (there are of course fast libraries too).<br/>
Among the flaws of these "slow" libraries, we find:
   - Poorly written loops / memory management: some functions require only a few minutes of work to run in a fraction of the time.
   - Lack for the more complex but also more efficient algorithms, including libraries that claim they have implemented these optimizations.

**This project is an attempt at providing a better compromise**:<br/>
 - Simplicity is the main goal but not at the cost of seeing programs run until the death of the Sun. This is very much a work in progress and a lot is yet to be implemented; based on the history of GMP, it may never end.
 - While we are at it, we aim at providing a more comprehensive documentation for the less sophisticated readers. We hope to make it a good introduction for those who have not dived into research papers yet.

## ToC

1. [Highlights](.#Highlights)
1. [Usage](.#Usage)
1. [Documentation](.#Documentation)
1. [License](.#License) 

## Highlights

- Modern C++ (compiles with C++11 / C++14 / C++17 / c++20).
- Simple operations:  
  - Artithmetic: `+`, `-`, `*`, `/`, `%`
  - Comparison: `==`, `!=`, `<=`, `>=`, `<`, `>`
  - Bitwise operations: `<<`, `>>`
- Advanced functions such as factorials.

More to come in the future.

## Usage

## Documentation

### Basics

[Type definition](https://github.com/AtmoFX/bigint/blob/master/documentation/basics.md)<br/>
Defined inside the `bigint` namespace, use arbitrarily large integers with the `bigint_t` class.
Big integers can be created from regular integer types or from any `std::string` representing an integer, then manipulated normally:
```c++
using namespace bigint;
bigint_t a(123456789), b(987654321),
          c = a + b, d = c * c;

std::cout << d;
```

### Operators

 Arithmetic operations working for regular integers are overloaded to work with big integers. The algorithms behind these operations are aimed to have fast algorithmic complexity. 
 Because calculating numbers without any way to output them would make the library rather useless, stream operators render big integers as strings.

[Comparison](https://github.com/AtmoFX/bigint/blob/master/documentation/comparison.md)<br/>
Big integers of any size can be compared to one another.
```c++
using namespace bigint;
bigint_t a("123456789123456789123456789123456789"),
          b("987654321987654321987654321987654321987654321");
bool e  = (a == b),  //false
     ne = (a != b),  //true
     gt = (a >  b),  //false
     ge = (a >= b),  //false
     lt = (a <  b),  //true
     le = (a <= b);  //true
//Since C++20
auto ss = (a <=> b); //std::strong_ordering::less
```

[Addition / Subtraction](https://github.com/AtmoFX/bigint/blob/master/documentation/addition_subtraction.md)<br/>
Add/subtract big integers normally:
```c++
bigint_t x = a + b, y = c - d;
```

Bit-wise operations<br/>
In progress.

[Multiplication](https://github.com/AtmoFX/bigint/blob/master/documentation/multiplication.md)<br/>
Multiply big integers normally. Multiplication is a complex operation that deserves its own separate documentation.
```c++
bigint_t x = a * b;
```

Division / Mod<br/>
In progress.

Input/Output of a `bigint_t`<br/>
In progress.
 
 ### Advanced algorithms

 The `bigint` namespace is shipped with more complex algorithms, with a non-obvious approach to ensure it outperforms naive implementations.

[Power](https://github.com/AtmoFX/bigint/blob/master/documentation/power.md)<br/>
The power function calculates $n^p = \overbrace{n \times n \times n \times \dotsc \times n}^{p\text{ times}}$.<br/>
This is done with a complexity of $\text{O}(\text{log}(p))$.
```c++
auto np bigint::power(123456789ULL, 62125); // 123456789 ^ 62125 
```

[Factorials](https://github.com/AtmoFX/bigint/blob/master/documentation/factorial.md)<br/>
The factorial function calculates $n! = 1 \times 2 \times 3 \times \dotsc \times n$ (typed: `size_t`).<br/>
The function uses about half the multiplications that would normally be required for this calculation.
```c++
auto f = bigint::factorial(100000); // 100k!
```

[Fibonacci sequence + generalization](https://github.com/AtmoFX/bigint/blob/master/documentation/fibonacci.md)<br/>
The Fibonacci sequence is a very famous sequence of integers, supported only up to its 93<sup>rd</sup> element when using 64-bit unsigned integers. `bigint::fibonacci` can go way, way beyond that point using an elaborate algorithm to diminish the calculation time as much as possible.<br/>
The algorithm is designed to produce consecutive Fibonacci numbers between 2 indices and handles 2 types of generalization of the Fibonacci sequence:
  - With custom values as first elements in the series.<br/>
  This allows the algorithm to generate Lucas numbers, other Fibonacci-like sequences and to have a stop&resume capability.
  - With higher order: generate a sequence where each number is the sum of the $k$, instead of only 2, elements preceding it.

```c+
auto f = bigint::fibonacci(500000);
```

## License

This library and its associated documentation are separately provided under [MIT license](https://github.com/AtmoFX/bigint/blob/master/License.md).
