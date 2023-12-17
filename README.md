# bigint

C++ header for big integers, with explanations.

To get started, download the [single header file](https://github.com/AtmoFX/bigint/blob/main/bigint.h), include it in your code, and compile however you would.

## A summary of the project

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
1. [License](.#License) 

## Highlights

- Modern C++ (compiles with C++11 / C++14 / C++17).
- Simple operations:  
  - Artithmetic: `+`, `-`, `*`, `/`, `%`
  - Comparison: `==`, `!=`, `<=`, `>=`, `<`, `>`
  - Bitwise operations: `<<`, `>>`
- Complex functions:
  - Factorial
  - Fibonacci (of any order)

More to come in the future.

## License

This library is provided under [MIT license](https://github.com/AtmoFX/bigint/blob/main/License.md).
