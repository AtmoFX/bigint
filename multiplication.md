# Multiplication

Development-wise, multiplication is an interesting operation. Finding the most efficient algorithm has been a decades-long research by many mathematicians, and has resulted in algorithms suitable for different size of integrs. This library relies on several algorithms to perform multiplication of 2 numbers:
- [Long multiplication](https://en.wikipedia.org/wiki/Multiplication_algorithm#Long_multiplication):<br/>
This is how we learn multiplications with several-digit-long numbers in school by taking each pair of digits from both operands, and multiply them together, with a carry.<br/>
Computational complexity is $\text{O}(\log(n)^2)$<br/>
Note that out of all the algorithms used to perform multiplication, this one is the only one that performs actual integer products on the processor(other algorithms simply break down numbers into smaller units). For that reason, while the computational complexity is fixed, ensuring the raw performance of the method is as high as can be is crucial.
- [Karatsuba](https://en.wikipedia.org/wiki/Karatsuba_algorithm ) (aka Toom-2):<br/>
When $log(n)$ doubles, the Karatsuba algorithm trades 1 multiplication for several additions/subtractions, thus making it 3 multiplications instead of 4.
The karatsuba algorithm is not suitable for multiplying (relatively) small numbers: it needs number of a certain side to make the 1 multiplication vs several additions/subtractions worth. <br/>
The complexity is $\text{O}(\log(n)^{\log_2(3)})$, or about $\text{O}(\log(n)^{1.585})$
- [Toom–Cook](https://en.wikipedia.org/wiki/Toom%E2%80%93Cook_multiplication) (aka Toom-3):<br/>
This method is not implemented yet.<br/>
The Toom-Cook algorithm is asymptoticly better than Karatsuba. For intermediate-size number, it returns a result in a shorter time; however, for the same reason the Karatsuba algorithm was not suitable for very small numbers, there exists a threshold under which Toom-Cook algorithm should not be used.
- [Schönhage–Strassen](https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm):<br/>
This method is not implemented yet.<br/>
This is yet another algorithm more efficient that the ones above, but this time, applicable for large numbers only.

The way the library performs multiplications is by checking the length of each operand and choosing the suitable algorithm to use. As recursive calls are done on smaller numbers, the library reevaluates what algorithm is suitable for their length. Eventually, the recursion ends with the long multiplication being applied to a sufficiently short segment of each operand.

This is the same approach as every other library that implement multiplication through several algorithms, the only differences being what algorithms were implemented, small details inside them and the thresholds to decide which one to run.

## Multiplication function

The multiplication function compares the size of the operands passed to it with hardcoded threshold, which are the best guess as what values give the shortest calculation time:

1. If $\text{size}(a) > \text{threshold}_\text{Schönhage–Strasse}$, return the product calculated using the Schönhage–Strasse algorithm.
1. If $\text{size}(a) > \text{threshold}_\text{Toom-3}$, return the product calculated using the Toom-3 algorithm.
1. If $\text{size}(a) > \text{threshold}_\text{Karatsuba}$, return the product calculated using the Karatsuba algorithm.
1. Otherwisse, return the product calculated using the long multiplication algorithm.


## Long multiplication (aka vanilla multiplication)

The long multiplication algorithm works on the principle that:

$$
\begin{align*}
a * b = \Big(\sum_i 2^{32i} \times a_i \Big) \times \Big(\sum_j 2^{32j} \times b_j \Big)  = \sum_{i, j} \big(2^{32(i+j)} \times a_i \times b_j\big)
\end{align*}
$$

The implementation is straight-forward. Using limbs of $n$ indexed from $0$ to $\text{size}(n) - 1$:
1. Initialize the variable $p$ to store the product, so that $\text{size}(p) = \text{size}(a) + \text{size}(b)$, with all its limbs initialized as $0$.
2. Start a for-loop: $i$ from $0$ to $\text{size}(a) - 1$
3. Initialize a variable to store the carry modulo $2^{32}$: $c \leftarrow 0$.
4. Start a nested for-loop: $j$ from $0$ to $\text{size}(a) - 1$.<br/>
Do $p_{i+j} \leftarrow \big(p_{i+j} + a_i \times b_j + c\big) \bmod 2^{32}$ and $c \leftarrow \lfloor {\big(p_{i+j} + a_i \times b_j + c\big)} / {2^{32}} \rfloor$.<br/>
In practice, these 2 operations are done by doing $\big(\text{result}_{i+j} + a_i \times b_j + c\big)$ into a 64-bit integer[^1], then splitting the integer into $\text{result}$ and $c$.
5. At the end of the loop, a carry may remain. Do $p_{i+j-1} \leftarrow c$.
6. Trim $p$ if the carry was 0.

[^1]: As determined in the [basics](./Basics.md#capacity-requirements-for-mathematical-operations), a 64-bit integer is enough to store the result of any 32-bit combination of the form: $a \times b + c + d$.

## Karatsuba algorithm

The principle of the Karatsuba algorithm is to find $k, k < \text{size}(a) \text{ and } k < \text{size}(b)$, which in practice is chosen as half the size of the operands, and divide the above multiplication as:

$$
\begin{align*}
a \times b =  & \Bigg( \Big(\sum_{i < k} 2^{32i} \times a_i \Big) + 2^{32k} \times \Big(\sum_{i \geq k} 2^{32(i-k)} \times a_i \Big) \Bigg) \times \Bigg( \Big(\sum_{j < k} 2^{32i} \times b_j \Big) + 2^{32k} \times \Big(\sum_{j \geq k} 2^{32(j- k)} \times b_j \Big) \Bigg)
\end{align*}
$$

For convenience, the 4 sums appearing in the above formula will be noted $\prescript{0}{}{a}, \prescript{k}{}{a}, \prescript{0}{}{b}, \prescript{k}{}{b}$.

$$
\begin{align*}
a \times b = & \big( \prescript{0}{}{a} + 2^{32k} \times \prescript{k}{}{a}) \times \big( \prescript{0}{}{b} + 2^{32k} \times \prescript{k}{}{b}) \\
= & \prescript{0}{}{a} \times \prescript{0}{}{b} + 2^{2 \times 32k} \times \prescript{k}{}{a} \times \prescript{k}{}{b} + 
2^{32k} \times \big(\prescript{0}{}{a} \times \prescript{k}{}{b} + \prescript{k}{}{a} \times \prescript{0}{}{b} \big) \\
= & \prescript{0}{}{a} \times \prescript{0}{}{b} + 2^{2 \times 32k} \times \prescript{k}{}{a} \times \prescript{k}{}{b} + 
2^{32k} \times \Big(\big(\prescript{0}{}{a} + \prescript{k}{}{a} \big) \times \big(\prescript{0}{}{b} + \prescript{k}{}{b} \big) - \prescript{0}{}{a} \times \prescript{0}{}{b} - \prescript{k}{}{a} \times \prescript{k}{}{b} \Big)
\end{align*}
$$

Although it looks like this just made the formula longer and more complicated, hence longer to calculate, the key to the algorithm is about being able to reuse $\prescript{0}{}{a} \times \prescript{0}{}{b}$ and $\prescript{k}{}{a} \times \prescript{k}{}{b}$ without calculating them a second time.<br/>
By doing so, we allow the Karatsuba algorithm to only triple the number of multiplications required when the length of the operands doubles, versus 4 multiplications for the long multiplication algorithm.

This 1 operation being optimized away at every step is a big deal: if we were to assume that at a given size $s$, the cost of the long multiplication is 10 (of an arbitrary unit) vs. 100 for the Karatsuba algorithm (including the additional operations, allocation of memory, ...), the table below shows how it evolves for each doubling of $s$.

Size | Cost (Long multiplication) | Cost (Karatsuba) | Ratio
:---:|---:|---:|:---:
$s$|$10$|$100$|$1:10$
$2s$|$40$|$300$|$1:7.5$
$4s$|$640$|$2{,}700$|$1:4.22$
$8s$|$40{,}960$|$72{,}900$|$1:1.78$
$16s$|$10{,}485{,}760$|$5{,}904{,}900$|$1.78:1$
$32s$|$1.07 \times 10^{10}$|$1.43 \times 10^9$|$7.48:1$
$64s$|$4.40 \times 10^{13}$|$1.05 \times 10^{12}$|$42.04:1$
$128s$|$7.21E \times 10^{17}$|$2.29 \times 10^{15}$|$314.98:1$
$256s$|$4.72 \times 10^{22}$|$1.50E \times 10^{19}$|$3146.26:1$


The algorithm consists in:

1. Initialize the variable $p$ to store the product, so that $\text{size}(p) = \text{size}(a) + \text{size}(b)$, with all its limbs initialized as $0$.
2. Calculate $k = \lceil \max(\text{size}(a), \text{size}(b)) /2 \rceil$.<br/>
This allows to obtain iterators for $a$, $b$ and $p$ as:
    - $\prescript{0}{}{a}$ and $\prescript{k}{}{a}$,
    - $\prescript{0}{}{b}$ and $\prescript{k}{}{b}$,
    - $\prescript{0}{}{p}$, $\prescript{k}{}{p}$ and $\prescript{2k}{}{p}$.<br/>
    Note that like the splits of $a$ and $b$, what is going to be stored in $\prescript{0}{}{p}$ and $\prescript{2k}{}{p}$ do not overlap but unlike them, $\prescript{k}{}{p}$ does overlap with the other two.
3. Do $\prescript{0}{}{p} \leftarrow \prescript{0}{}{a} \times \prescript{0}{}{b}$, by a recursive call to the multiplication algorithm.
4. Do $\prescript{2k}{}{p} \leftarrow \prescript{k}{}{a} \times \prescript{k}{}{b}$ the same way.
5. Initialise a variable $pk$, of size $2k$, filled with zeroes.
6. Do $pk \leftarrow \big(\prescript{0}{}{a} + \prescript{k}{}{a} \big) \times \big(\prescript{0}{}{b} + \prescript{k}{}{b} \big) - \prescript{0}{}{a} \times \prescript{0}{}{b} - \prescript{k}{}{a} \times \prescript{k}{}{b}$, using a last recursive call to the multiplication function.
7. Finally, do $\prescript{k}{}{p} \leftarrow \prescript{k}{}{p} + pk$.
