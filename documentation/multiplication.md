# Multiplication

Development-wise, multiplication is an interesting operation because of its time complexity.<br/>
The case to consider is the multiplication of 2 numbers $a$ and $b$ of roughly the same size, i.e. each made of a certain number $l$ of limbs. If e.g. $a$ was made of only 1 limb the complexity for the multiplication would simply be $\text{O}(l)$ (each limb of $b$ must be multiplied by $a$). Being limited to such a case would not make things interesting.

In practice, operands rarely are of the exact same size but algorithms can easily cope with any difference and the theoretical considerations can be solved by filling the smaller number with leading zeroes.<br/>
With $p = a \times b$, we note $l = \max(\text{size}(a), \text{size}(b))$.


- [Long multiplication](https://en.wikipedia.org/wiki/Multiplication_algorithm#Long_multiplication):<br/>
This is how we learn multiplications in school, by taking each pair of digits from both operands, and multiply them together, with a carry.<br/>
The computational complexity is $\text{Θ}(l^2)$.<br/>
The other algorithms (addition, subtraction, ...) having a $\text{O}(l)$  complexity, they are practically free in comparison.

    The demonstration that better algorithms exist has triggered a decades-long effort by mathematicians to improve performance further and further. This resulted in several algorithms, each better than the previous and suitable for increasingly big numbers.
- [Karatsuba](https://en.wikipedia.org/wiki/Karatsuba_algorithm ):<br/>
Each time $l$ doubles, the Karatsuba algorithm trades 1 multiplication for several additions/subtractions, thus making it 3 multiplications instead of 4.
The karatsuba algorithm is not suitable for multiplying (relatively) small numbers: it needs number of a certain side to make the 1 multiplication vs several additions/subtractions worth. <br/>
The complexity is $\text{Θ}(l^{\log_2(3)}) = \text{O}(l^{1.585})$.
- [Toom-Cook](https://en.wikipedia.org/wiki/Toom%E2%80%93Cook_multiplication) algorithms:<br/>
The Toom-Cook algorithms form a family of divide-and-conquer algorithms similar to Karatsuba, except for the fact they are not limited to splitting operands in halves. The smaller the parts, the more multiplications it can trade against simpler operations. For this reason, each Toom-Cook algorithm needs longer and longer numbers to make the trade worth it.<br/>
The complexity for Toom-3 is $\text{Θ}(l^{\log_3(5)}) = \text{O}(l^{1.465})$.

- [Schönhage–Strassen](https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm):<br/>
This method is not implemented yet.<br/>
This is yet another algorithm more efficient that the ones above, but this time, applicable for large numbers only, with millions of digits.

## Multiplication function

The multiplication function compares the size of the operands passed to it with hardcoded threshold, which are the best guess as what values give the shortest calculation time:

1. If $\text{size}(a) > \text{threshold}_\text{Schönhage–Strassen}$<br/>
    $\rightarrow$ return the product calculated using the Schönhage–Strassen algorithm.
1. If $\text{size}(a) > \text{threshold}_\text{Toom-3}$<br/>
    $\rightarrow$ return the product calculated using the Toom-3 algorithm.
1. If $\text{size}(a) > \text{threshold}_\text{Karatsuba}$<br/>
    $\rightarrow$ return the product calculated using the Karatsuba algorithm.
1. Otherwise<br/>
    $\rightarrow$ return the product calculated using the long multiplication algorithm.

This is the same approach as every other library that implement multiplication through several algorithms, the only differences being what algorithms were implemented, small details inside them and the thresholds to decide which one to run.

## Long multiplication

The long multiplication algorithm works on the principle that:

$$
\begin{flalign*}
a * b = \Big(\sum_i 2^{32i} \times a_i \Big) \times \Big(\sum_j 2^{32j} \times b_j \Big)  = \large \sum_i \normalsize \sum_j \big(2^{32(i+j)} \times a_i \times b_j\big) &&
\end{flalign*}
$$

The implementation is straight-forward. Indexing limbs of operands starting at index 0:

1. Initialize the variable $p$ to store the product, so that $2 l = \text{size}(p) = \text{size}(a) + \text{size}(b)$, with all its limbs initialized as $0$.
2. Start a for-loop: $i$ from $0$ to $\text{size}(a) - 1$
3. Initialize a variable to store the carry modulo $2^{32}$: $c \leftarrow 0$.
4. Start a nested for-loop: $j$ from $0$ to $\text{size}(a) - 1$.<br/>
Do $p_{i+j} \leftarrow \big(p_{i+j} + a_i \times b_j + c\big) \bmod 2^{32}$ and $c \leftarrow \lfloor {\big(p_{i+j} + a_i \times b_j + c\big)} / {2^{32}} \rfloor$.<br/>
In practice, these 2 operations are done by doing $\big(\text{result}_{i+j} + a_i \times b_j + c\big)$ into a 64-bit integer[^1], then splitting the integer into $\text{result}$ and $c$.
5. At the end of the outer loop, a carry may remain. Do $p_{2l-1} \leftarrow c$.
6. Trim $p$ if the carry was 0.

[^1]: As determined in the [basics](./basics.md), a 64-bit integer is enough to store the result of any 32-bit combination of the form: a x b + c + d.

## Karatsuba algorithm

Karatsuba, from the name of its inventor, is a divide-and-conquer algorithm. Its principle run the multiplication as:

$$
\begin{flalign*}
a \times b =  & \Bigg( \Big(\sum_{i < \lceil l/2 \rceil} 2^{32i} \times a_i \Big) + 2^{32\lceil l/2 \rceil} \times \Big(\sum_{i \geq \lceil l/2 \rceil} 2^{32(i-\lceil l/2 \rceil)} \times a_i \Big) \Bigg) \times \Bigg( \Big(\sum_{j < \lceil l/2 \rceil} 2^{32i} \times b_j \Big) + 2^{32\lceil l/2 \rceil} \times \Big(\sum_{j \geq \lceil l/2 \rceil} 2^{32(j- \lceil l/2 \rceil)} \times b_j \Big) \Bigg) &&
\end{flalign*}
$$

For convenience, the 4 sums, i.e. substrings of the integers' limbs, appearing in the above formula will be noted with Greek letters: $\alpha_0, \alpha_1, \beta_0, \beta_1$ (we keep letters from the Latin alphabet to represent single limbs of a number).

$$
\begin{flalign*}
a \times b = & \big( {\alpha_0} + 2^{32\lceil l/2 \rceil} \times \alpha_1) \times \big( \beta_0 + 2^{32\lceil l/2 \rceil} \times \beta_1) & \\
= & \alpha_0 \times \beta_0 + 2^{64\lceil l/2 \rceil} \times \alpha_1 \times \beta_1 + 
2^{32\lceil l/2 \rceil} \times \big(\alpha_0 \times \beta_1 + \alpha_1 \times \beta_0 \big) \\
= & \alpha_0 \times \beta_0 + 2^{64\lceil l/2 \rceil} \times \alpha_1 \times \beta_1 + 
2^{32\lceil l/2 \rceil} \times \Big(\big(\alpha_0 + \alpha_1 \big) \times \big(\beta_0 + \beta_1 \big) - \alpha_0 \times \beta_0 - \alpha_1 \times \beta_1 \Big)
\end{flalign*}
$$

Although it looks like this just made the formula longer and more complicated, hence longer to calculate, the key to the algorithm is about being able to reuse $\alpha_0 \times \beta_0$ and $2^{64\lceil l/2 \rceil} \times \alpha_1 \times \beta_1$ where necessary.<br/>
By doing so, we allow the Karatsuba algorithm to execute in 3 multiplications what the long multiplication would do in 4.

This 1 operation being optimized away at every step is a big deal: if we were to assume that at a given size $s$, the cost of the long multiplication is 1 (of an arbitrary unit) vs. 5 for the Karatsuba algorithm (including the additional operations, allocation of memory, ...), the table below shows how it evolves for each doubling of $s$.

Size | Long multiplication | Cost of Karatsuba | Ratio
:---:|---:|---:|:---:
1s|1|5|1:5
2s|4|15|1:3.75
4s|16|45|1:2.81
8s|64|135|1:2.11
16s|256|405|1:1.58
32s|1,024|1,215|1:1.19
64s|4,096|3,645|1.12:1
128s|16,384|10,935|1.50:1
256s|65,536|32,805|2.00:1
512s|262,144|98,415|2.66:1
1024s|1,048,576|295,245|3.55:1

The Karatsuba algorithm consists in:

1. Initialize the variable $p$ to store the product, so that $2l = \text{size}(p) = \text{size}(a) + \text{size}(b)$, with all its limbs initialized as $0$.
2. Split $a$ a,d $b$ at the limb indexed $k = \lceil l /2 \rceil$.<br/>
This allows to obtain the subparts of $a$, $b$ and $p$ as:
    - $\alpha_0$ and $\alpha_1$,
    - $\beta_0$ and $\beta_1$,
    - $\psi_0$, $\psi_1$ and $\psi_2$.<br/>
    Note that $\psi_0$ and $\psi_2$ do not overlap in $p$. However, $\psi_1$ does overlap with the other two in memory.
3. Do $\psi_0 \leftarrow \alpha_0 \times \beta_0$, by a recursive call to the multiplication algorithm.
4. Do $\psi_2 \leftarrow \alpha_1 \times \beta_1$ the same way.
5. Initialiée a variable $p_\text{mid}$, of size $l$.
6. Do $p_\text{mid} \leftarrow \big( \alpha_0 + \alpha_1 \big) \times \big(\beta_0 + \beta_1 \big) - \alpha_0 \times \beta_0 - \alpha_1 \times \beta_1$.
7. Finally, do $\psi_1 \leftarrow \psi_1 + p_\text{mid}$.

## Toom-Cook

Toom-Cook is a divide-and-conquer algorithm (rather, an infinite family of algorithms) named after its two discoverers: Andrei Toom who constructed a method for multiplication with  and Stephen Cook who turned it in an algorithm. The method produces an infinite family of algorithms.<br/>
Just like Karatsuba, Toom-Cook swaps some of the required multiplications for a finite number of simpler operations, all of which are $\text{O}(l)$ in complexity.

The more the operands are broken down (Toom-2 is equivalent to Karatsuba, Toom-3 breaks them down into 3, Toom-4 breaks them down into 4, etc), the better the complexity. However, this also comes at the cost of having more and more of the simpler operations. Hence, doing so requires increasingly bigger numbers to make the trade worth it. 

The complexity of Toom-Cook, when breaking down operands of length $l$ into $k$ parts of length $l/k$, is $\text{Θ}(l^{\log_k(2k-1)})$.

Toom-k| Complexity | Approximate
:---: | :---:|:---:
2|$\text{Θ}(l^{\log_2(3)})$| $\text{O}(l^{1.585})$
4|$\text{Θ}(l^{\log_3(5)})$| $\text{O}(l^{1.465})$
5|$\text{Θ}(l^{\log_4(7)})$| $\text{O}(l^{1.404})$
6|$\text{Θ}(l^{\log_5(9)})$| $\text{O}(l^{1.366})$
...|...|...
10|$\text{Θ}(l^{\log_{10}(19)})$| $\text{O}(l^{1.279})$
20|$\text{Θ}(l^{\log_{20}(39)})$| $\text{O}(l^{1.223})$
50|$\text{Θ}(l^{\log_{50}(99)})$| $\text{O}(l^{1.175})$
100|$\text{Θ}(l^{\log_{100}(199)})$| $\text{O}(l^{1.150})$

Because of the diminishing returns, Toom-Cook algorithms are not implemented for large values of $k$, generally not above Toom-4 or Toom-5.<br/>
For the sake of comparison, when the size of the operands is multiplied by 4, it generates:
 - for the long division, 16 times as many multiplications.
 - for Karatsuba, 9 times as many multiplications.
 - for Toom-4, 7 times as many multiplications (not counting the multiplications by small constants due to their $\text{O}(l)$ complexity as discussed earlier).


### Principle of Toom-Cook

To understand Toom-Cook algorithms, we have to express big integers as the result of polynomials. Since big integers are encoded as limbs, it looks natural to express
$a = \displaystyle \sum_{i \geq 0}  \limits a_i \times 2^{32i}$
as the result of a polynomial: $a = A(2^{32})$
where $A(x) = \displaystyle \sum_{i \geq 0}  \limits a_i \times x^i$.

This is not a polynomial expression we can use here, though. Toom-k splits operands in $k$ parts so we need to express them as a polynomial with $k$ coefficients, i.e. of degree $k-1$:<br/>
$A(x) = \displaystyle \large \sum_{i=0}^{k-1} \bigg( \normalsize \sum_{j=0}^{\lceil l/k \rceil-1} a_{j + i \times \lceil l/k \rceil} \times 2^{32j} \bigg) \times x^i$, that we note $A(x) = \displaystyle \sum_{i=0}^{k-1} \alpha_i \times x^i$

The product $p = a \times b$ is now replaced by the polynomial $P$ of degree $2(k-1)$.

Obviously, we are not doing it to just calculate the direct product of $A(x)$ and $B(x)$, each coefficient at a time.

#### Polynomial interpolation

Thankfully, [polynomial interpolation](https://en.wikipedia.org/wiki/Polynomial_interpolation) allows to calculate a polynomial of degree $2(k-1)$ using only $2k-1$ points.

We will do exactly that with easy-to-calculate points.<br/>
The chosen points are 0 and numbers in the form of $\pm 2^i, i \geq 0$ since they are the easiest to deal with for a computer. 


For Toom-3, we need 5 points, that we actually calculate:

$$
\begin{flalign*}
\begin{cases}
P(\text{-}2) & = & A(\text{-}2) & \times & B(\text{-}2) & = & (4\ \alpha_2 - 2\ \alpha_1 + \alpha_0) & \times & (4\ \beta_2 - 2\ \beta_1 + \beta_0) \\
P(\text{-}1) & = & A(\text{-}1) & \times & B(\text{-}1)  & = & (\alpha_2 - \alpha_1 + \alpha_0) & \times & (\beta_2 - \beta_1 + \beta_0)\\
P(0) & = & A(0) & \times & B(0) & = & \alpha_0 & \times & \beta_0 \\
P(1) & = & A(1) & \times & B(1) & = & (\alpha_2 + \alpha_1 + \alpha_0) & \times & (\beta_2 + \beta_1 + \beta_0) \\
P(2) & = & A(2) & \times & B(2) & = & (4\ \alpha_2 + 2\ \alpha_1 + \alpha_0) & \times & (4\ \beta_2 + 2\ \beta_1 + \beta_0)
\end{cases} &&
\end{flalign*}
$$

We have now replaced the product of 2 operands of length $l$ by 5 products, each with operands of length $l/3$ (hence the complexity).

#### Linear equations

Remembering that $P(x) = \psi_4 \times x^4 + \psi_3 \times x^3 + \psi_2 \times x^2 + \psi_1 \times x + \psi_0$, we have a system of 5 linear equations:

$$
\begin{flalign*}
\begin{cases}
\psi_0 & = & P(0) \\
16\ \psi_4 - 8\ \psi_3 + 4\ \psi_2 - 2\ \psi_1 + \psi_0 & = &  P(\text{-}2) \\
\psi_4 - \psi_3 + \psi_2 - \psi_1 + \psi_0 & = &  P(\text{-}1) \\
\psi_4 + \psi_3 + \psi_2 + \psi_1 + \psi_0 & = &  P(1) \\
16\ \psi_4 + 8\ \psi_3 + 4\ \psi_2 + 2\ \psi_1 + \psi_0 & = &  P(2) \\
\end{cases} &&
\end{flalign*}
$$ 

We can solve it and deduce the values $\psi_i$ but was it the best we could do?<br/>
Well... no and while it won't improve the complexity, we can slightly simplify the above equations.

The clue is to note that when $x \rightarrow \infty$, $P(x) \displaystyle \equiv \psi_4 \times x^4$ and $A(x) \times B(x) \equiv (\alpha_2 \times x^2) \times (\beta_2 \times x^2) = \alpha_2 \times \beta_2 \times x^4$.

The asymptotic behavior of $P$ results in a new "point", usually noted $P(\infty)$, defined as $P(\infty) = \psi_4 = \alpha_2 \times \beta_2$.<br/>
This is much simpler than the above equations and gives $\psi_4$ directly. As such, it avoids some calculations.

Now, we can replace one of the points chosen above by $P(\infty)$. For instance, if $P(\text{-}2)$ is discarded.

$$
\begin{flalign*}
\begin{cases}
\psi_0 & = & P(0) \\
\psi_4 & = & P(\infty) \\
\psi_4 - \psi_3 + \psi_2 - \psi_1 + \psi_0 & = &  P(\text{-}1) \\
\psi_4 + \psi_3 + \psi_2 + \psi_1 + \psi_0 & = &  P(1) \\
16\ \psi_4 + 8\ \psi_3 + 4\ \psi_2 + 2\ \psi_1 + \psi_0 & = &  P(2) \\
\end{cases} &&
\end{flalign*}
$$ 

Which we solve into the following system, written in a way that makes it as convenient as possible for a computer:

$$
\begin{flalign*}
\begin{cases}
\psi_0 & = P(0) \\
\psi_4 & = P(\infty) \\
\psi_2 & = \big(P(\text{-}1) + P(1)\big) / 2 - \big(\psi_0 + \psi_4\big) \\
\psi_3 & = \big[\psi_0 - 14\ \psi_4 + P(2) - 2\ \big(\psi_2 + P(1)\big)\big] / 6 \\
\psi_1 & = P(1) - \big(\psi_0 + \psi_4 \big) - \big(\psi_2 + \psi_3\big)
\end{cases} &&
\end{flalign*}
$$ 


If it was $P(2)$ that was replaced by $P(\infty)$, the resulting coefficients would be expressed as:

$$
\begin{flalign*}
\begin{cases}
\psi_0 & = P(0) \\
\psi_4 & = P(\infty) \\
\psi_2 & =  \big(P(\text{-}1) + P(1)\big)/2 - \psi_0 - \psi_4 \\
\psi_3 & =  \big[\text{‒}\ \psi_0 + 14\ \psi_4 - P(\text{-}2) + 2\ \big(\psi_2 + P(\text{-}1)\big)\big] / 6\\
\psi_1 & = \big(P(1) - P(\text{-}1)\big)/2 - \psi_3\\
\end{cases} &&
\end{flalign*}
$$ 


Note that if you were to finish solving the equation system (i.e. get rid of the $\psi_i$ on the right of the equal signs), you would end up with equalities that could be expressed in the form of a product of matrices.<br/>
This matrix product is how the solution to Toom-3 is presented on Wikipedia but it is not completely optimal in terms of number of operations to execute.

$$
\begin{pmatrix}
\psi_0 \\
\psi_1 \\
\psi_2 \\
\psi_3 \\
\psi_4
\end{pmatrix} =
\begin{pmatrix}
1 & 0 & 0 & 0 & 0 \\
{^1\negmedspace/\negmedspace_2} &  {^1\negmedspace/\negmedspace_3} & \text{-}1 & {^1\negmedspace/\negmedspace_6} & \text{-}2 \\
\text{-}1 & {^1\negmedspace/\negmedspace_2} & {^1\negmedspace/\negmedspace_2} & 0 & \text{-}1 \\
{^{\text{-}1}\negmedspace/\negmedspace_2} & {^1\negmedspace/\negmedspace_6} & {^1\negmedspace/\negmedspace_2} & {^{\text{-}1}\negmedspace/\negmedspace_6} & 2 \\
 0 & 0 & 0 & 0 & 1
\end{pmatrix} \times
\begin{pmatrix}
P(0) \\
P(1) \\
P(\text{-}1) \\
P(\text{-}2) \\
P(\infty)
\end{pmatrix}
$$

### Example application

As big integers are coded with limbs in base $2^{32}$, the actual algorithm works in a base of the form ${2^{32}}^i$. Let alone numbers big enough for the algorithm to be efficient (they would need thousands of digits), even i = 1 would make the numbers too big for an illustration; we are talking about multiplying 2 numbers whose scale is in the octillions (US/short form) or thousand quadrillions (European/long form), with a resulting product in the scale of Septendecillion (US) or Nonillion (European), i.e. over 50 base-10 digits. 

For clarity's sake, the illustration is done with numbers under 10<sup>9</sup>, in a base 10<sup>k</sup> (convenient for human beings), then in a base 2<sup>k</sup> (convenient for computers).<br/>

$a = 123{,}456{,}789, \nobreakspace b=987{,}654{,}321$

#### If limbs were expressed in base 10

Since $\lceil \log_{10}(a) / 3 \rceil = \lceil \log_{10}(b) / 3 \rceil =  3$, we will work in base $10^3$:

$$
\begin{flalign*}
A(x) & = 123x^2 +456x + 789 & \big(a = A(10^3)\big)&& \\
B(x) & = 987x^2+654x+321 & \big(b = B(10^3)\big)
\end{flalign*}
$$

We calculate the 5 points $P(x) = A(x) \times B(x)$:

$$
\begin{flalign*}
P(\text{-}1) & = & 298{,}224 && \\
P(0) & = & 253{,}269 & \\
P(1) & = & 2{,}684{,}016 & \\
P(2) & = &   12{,}230{,}361 &  \\
P(\infty) & = & 121{,}401 & \\
\end{flalign*} \\
$$

Applying the formulas we obtained from the linear equations, we deduce that $P(x) = 121{,}401 x^4 +  530{,}514 x^3 + 1{,}116{,}450 x^2 +  662{,}382 x +  253{,}269$ and therefore, thanks to a sum of the shifted coefficients $\psi_i$, equivalent to multiplying them by ${10^3}^i$:

```math
\begin{flalign*}
\begin{array}{c}
  \tiny {10^0 \times} & & & & & & & & & & 253 & {,} & 269 & \\
+ \tiny {10^3 \times} & & & & & & & & 662 & {,} & 382 & & & \\
+ \tiny {10^6 \times} & & & & 1 & {,} & 116 & {,} & 450 & &  & & & \\
+ \tiny {10^9 \times} & & & & 530 & {,} & 514 & & & & & & & \\
+ \tiny {10^{12} \times} & & \nobreakspace 121 & {,} & 401 & & & & & & & \\
& \hline
P(10^3)= & 121 & {,} & 932 & {,} & 631 & {,} & 112 & {,} & 635 & {,} & 269
\end{array} & &
\end{flalign*}
```


#### If limbs were expressed in base 2

Since $\lceil \log_{2}(a) / 3 \rceil = 9$ and $\lceil \log_{2}(b) / 3 \rceil = 10$, we will work in base 2<sup>10</sup>:

$$
\begin{flalign*}
A(x) & = 117x^2 + 755x+277 & \big(a = A(2^{10})\big)&& \\
B(x) & = 941x^2+922x + 177 & \big(b = B(2^{10})\big) 
\end{flalign*}
$$

We calculate the 5 points $P(x) = A(x) \times B(x)$:

$$
\begin{flalign*}
P(\text{-}1) & = & \text{-}70{,}756 && \\
P(0) & = & 49{,}029 & \\
P(1) & = & 2{,}343{,}960 & \\
P(2) & = &   13{,}045{,}175 &  \\
P(\infty) & = & 110{,}097 & \\
\end{flalign*} \\
$$

Applying the formulas we obtained from the linear equations, we deduce that $P(x) = 110{,}097 x^4 +  818{,}329 x^3 +  977{,}476 x^2 +  389{,}029 x +  49{,}029$.<br/>
The sum of the coefficients, after shifting each by a multiple of 10 bits gives the program: $P(2^{10}) = 121{,}932{,}631{,}112{,}635{,}269$, the very same result as the calculation made in base 10<sup>3</sup>.
