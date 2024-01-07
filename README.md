# bigint

C++ header for big integers, with explanations.

To get started, download the [single header file](https://github.com/AtmoFX/bigint/blob/dev/bigint.h), include it in your code, and compile however you would.

## A summary of the project

**Do you need thousands of digits? Millions? More? This library got you covered!<br/>
Are you after documentation but cannot be bothered looking up for research papers? Browse the resources here.**

There are already many big integer libraries out there:

- GMP (The GNU Multiple Precision Arithmetic Library) is by far the most well-known, and not only for integers.
With more than 30 years since its first release, it has become the reference to achieve the best possible performance.<br/>
It is also a very complex piece of work, hard to include in projects (especially under Windows) and definitely not the easiest way to get an introduction to the underlying mathematics and algorithms.
- Other libraries have a different offer: much more simple but for most of them, painfully slow.

**This project is an attempt at providing a better compromise**: simplicity is the main goal but not at the cost of seeing programs run until the death of the Sun.<br/>
While we are at it, we aim at providing a more comprehensive documentation for the less sophisticated readers.

## ToC

1. [Highlights](.#Highlights)
1. [Usage](.#Usage)
1. [Documentation](.#Documentation)
1. [License](.#License) 

## Highlights

- Modern C++ (compiles with C++11 / C++14 / C++17).
- Simple operations:  
  - Artithmetic: `+`, `-`, `*`, `/`, `%`
  - Comparison: `==`, `!=`, `<=`, `>=`, `<`, `>`
  - Bitwise operations: `<<`, `>>`
- Advanced functions such as factorials.

More to come in the future.

## Usage

## Documentation

 - [About creating your own big integer library/algorithms](bigintcreation.md)<br/>
There are already many big integer libraries out there. If you are only interested in the document I have written to create you own, be my guest. However, here is a fair warning for you, implementing the 4 basic arithmetic operations is an extremely complex topic and to say the least:<br/> <font size="5">**It is not fun.**</font><br/><br/>
You really, really should not go with the assumption that implementing a $n \times p$ operation is easier than implementing $n^p$, just because the latter needs the former to work. It is quite the opposite in fact: the multiplication is extremely complex whereas a reasonably efficient power function can be implemented in minutes.<br/>
If all you want is test your own algorithms on big enough values, you should simply download this library (or another), skip all the pain and have fun with your functions.
 
 - [Basics](basics.md)<br/>
   Defined inside the `bigint` namespace, use arbitrarily large integers with the `bigint_t` class.
   Big integers can be created from regular integer types or from any `std::string` representing an integer, then manipulated normally:
    ```c++
    using namespace bigint;
    bigint_t a(123456789), b(987654321),
             c = a + b, d = c * c;

    std::cout << d;
    ```
 - Operators<br/>
 Arithmectic operations working for regular integers also work for big integers. The algorithms behind these operations are aimed to have fast algorithmic complexity. 
 Because calculating numbers without any way to output them would make the library rather useless, stream operators render big integers as strings.
   - [Comparison](comparison.md)<br/>
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
   - [Addition / Subtraction](addition_subtraction.md)<br/>
   Add/subtract big integers normally:
       ```c++
       bigint_t x = a + b, y = c - d;
       ```
   - Bit-wise operations
   - [Multiplication](multiplication.md)<br/>
   Multiply big integers normally. Multiplication is a complex operation that deserves its own separate documentation.
   ```c++
   bigint_t x = a * b;
   ```
   - Division / Mod
   - Input/Output of a `bigint_t`
 - Advanced algorithms<br/>
 The `bigint` namespace is shipped with more complex algorithms, with a non-obvious approach to ensure it outperforms naive implementations.
   - [Power](power.md)<br/>
   The power function calculates $n^p$, i.e. the product $n \times n \times n \times \dotsc \times n$ with itself, $n$ appearing a total of $p$ times.
      ```c++
      auto np bigint::power(123456789ULL, 62125); // 123456789 ^ 62125 
      ```
   - [Factorials](factorial.md)<br/>
   The factorial function calculates $n!$, i.e. the product of all natural numbers between 1 and a given $n$ (typed: `size_t`). 
      ```c++
      auto f = bigint::factorial(100000); // 100k!
      ```
   - [Fibonacci sequence + generalizations](fibonacci.md)<br/>
   The Fibonacci sequence is a very famous sequence of integers, supported only up to its $93^\text{rd}$ element when using 64-bit integers. `bigint::fibonacci` can go way, way beyond that point using an elaborate algorithm to diminish the calculation time as much as possible.<br/>
   The algorithm is designed to produce consecutive Fibonacci numbers between 2 indices and handles 2 types of generalization of the Fibonacci sequence:
     - With custom values as first elements in the series.<br/>
     This allows the algorithm to generate Lucas numbers, other Fibonacci-like sequences and to have a stop&resume capability.<br/>
     That latter use of the input parameters allows to greatly decrease memory usage when generating large portions of the sequence (demonstrated in the example below): break the portion down into smaller segments, only keeping the last 2 numbers and let the memory be cleaned as the other numbers go out of scope.
     - With higher order of numbers: generate a sequence where each number is the sum of the $k$, instead of only 2, elements preceding it.
   ```c+
   auto f = bigint::factorial(500000);
   ```

## License

This library is provided under [MIT license](https://github.com/AtmoFX/bigint/blob/main/License.md).
