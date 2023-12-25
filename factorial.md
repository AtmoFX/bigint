# Factorial

## Function signature

The factorial function is defined as:
```c++
bigint_t factorial(size_t n);
```

This calculates, obviously, $`n!`$.

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
 - It requires `n - 1` multiplications (from `2` to `n`) and importantly, some of these products are done more than once; with the algorithmic complexity of the multiplication, this is something to avoid.<br/>
For instance, we would want $`3`$, $`6`$, $`12`$, $`24`$ (i.e. $`3`$'s multiplied by a power of $`2`$) to be processed as $`64 \times (3^2)^2`$: This requires 2 multiplications (The multiplication by $`64`$ is done through bit shifting) instead of 3. 
 - The operands passed in the successive multiplications are very different in size.<br/>
 Having one of the operand consiting of 1 or 2 limbs prevents from using the more efficient multiplication algorithms.
 - As was mentioned above, we can make use of bit shifting operations when multiplying even numbers.<br/>
In fact, it is possible to account for all the $`2`$'s encountered between $`1`$ and $`n`$ with a single shift.

## The library's approach

### Principle

Instead of the naive implementation, the implemented algorithm works "backward", with only odd numbers ($`3`$, $`5`$, $`7`$, $`9`$ ...). All the even numbers will be managed at once, by shifting bits left.<br/>
For the sake of clarity, we will illustrate each step with $`n = 20`$. The table below represents the factors; when used, they get crossed out.

|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|

### Algorithm

1. Initialize the result variable $`\text{result} \leftarrow 1`$.
2. Determine $`p`$ so that $`2^p \times 3 \leq n`$. It is possible that $`2^p \times 5 \leq n`$.<br/>
Example: with $`n = 20`$: $`2^3 \times 3 = 24 \gt 20`$ and $`2^2 \times 3 = 12 \leq 20`$. This means $`p = 2`$.
3. Initialize a variable $`\text{product} \leftarrow 3`$.
4. Find the highest odd integer $`i`$ that verifies $`2^p \times i \leq n`$. With $`n = 20`$ and $`p = 2`$, that number is $`5`$ ($`2^2 \times 5 \leq 20`$).<br/>
For every **additional** odd number $`i`$ that verifies the above inequality, do $`\text{product} \leftarrow \text{product} \times i`$.
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

Compared to the naive approach and for big enough factorials:
 - The result is attained with half the number of multiplications required by the naive approach.<br/>
 The computational complexity is not improved by that but when every multiplication takes a measurable time, cutting their number by half is an important improvement.
 - It makes better use of the optimized multiplication algorithms, as both $`\text{product}`$ and $`\text{result}`$ grow.<br/>
 This part of the improvement results in better computational complexity: the algorithm repeats $`n`$ times a step with an overall better $`\text{O}(\text{M}(n))`$.

### A complete example

With $`n = 60`$:

$`\text{product} \leftarrow 1`$<br/>
$`\text{result} \leftarrow 1`$<br/>

1. First loop:<br/>
$`p \leftarrow 4`$ ($`2^p = 16 `$)<br/>
$`\text{product} \leftarrow \text{product} \times 3 = 3`$<br/>
$`\text{result} \leftarrow \text{result} \times \text{product} = 3`$<br/>
1 value, $`48`$, gets used up.

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|
|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|
|41|42|43|44|45|46|47|<strike>48</strike>|49|50|51|52|53|54|55|56|57|58|59|60|

2. Second loop:<br/>
$`p \leftarrow 3`$ ($`2^p = 8 `$)<br/>
$`\text{product} \leftarrow \text{product} \times 5 \times 7 = 105`$<br/>
$`\text{result} \leftarrow \text{result} \times \text{product} = 315`$<br/>
3 values, $`24`$, $`40`$ and $`56`$, get used up, with 3 multiplications done.

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|
|21|22|23|<strike>24</strike>|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|<strike>40|
|41|42|43|44|45|46|47|<strike>48</strike>|49|50|51|52|53|54|55|<strike>56</strike>|57|58|59|60|

3. Third loop:<br/>
$`p \leftarrow 2`$ ($`2^p = 4 `$)<br/>
$`\text{product} \leftarrow \text{product} \times 9 \times 11 \times 13 \times 15 = 2{,}027{,}025`$<br/>
$`\text{result} \leftarrow \text{result} \times \text{product} = 638{,}512{,}875`$<br/>
7 values, $`12`$, $`20`$, $`28`$, $`36`$, $`44`$, $`52`$ and $`60`$, get used up, with 5 multiplications done.

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1|2|3|4|5|6|7|8|9|10|11|<strike>12</strike>|13|14|15|16|17|18|19|<strike>20</strike>|
|21|22|23|<strike>24</strike>|25|26|27|<strike>28</strike>|29|30|31|32|33|34|35|<strike>36</strike>|37|38|39|<strike>40</strike>|
|41|42|43|<strike>44</strike>|45|46|47|<strike>48</strike>|49|50|51|<strike>52</strike>|53|54|55|<strike>56</strike>|57|58|59|<strike>60</strike>|

4. Fourth loop:<br/>
$`p \leftarrow 1`$ ($`2^p = 2`$)<br/>
$`\text{product} \leftarrow \text{product} \times 17 \times 19 \times 21 \times 23 \times 25 \times 27 \times 29 = 6{,}190{,}283{,}353{,}629{,}375 `$<br/>
$`\text{result} \leftarrow \text{result} \times \text{product} = 3{,}952{,}575{,}621{,}190{,}533{,}915{,}703{,}125`$<br/>
14 values, $`6`$, $`10`$, $`14`$, $`18`$, $`22`$, $`26`$, $`30`$, $`34`$, $`38`$, $`42`$, $`46`$, $`50`$, $`54`$ and $`58`$, get used up, with 8 multiplications done.

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1|2|3|4|5|<strike>6</strike>|7|8|9|<strike>10</strike>|11|<strike>12</strike>|13|<strike>14</strike>|15|16|17|<strike>18</strike>|19|<strike>20</strike>|
|21|<strike>22</strike>|23|<strike>24</strike>|25|<strike>26</strike>|27|<strike>28</strike>|29|<strike>30</strike>|31|32|33|<strike>34</strike>|35|<strike>36</strike>|37|<strike>38</strike>|39|<strike>40</strike>|
|41|<strike>42</strike>|43|<strike>44</strike>|45|<strike>46</strike>|47|<strike>48</strike>|49|<strike>50</strike>|51|<strike>52</strike>|53|<strike>54</strike>|55|<strike>56</strike>|57|<strike>58</strike>|59|<strike>60</strike>|

5. Fifth loop:<br/>
$`p \leftarrow 0`$ ($`2^p = 1`$)<br/>
$`\text{product} \leftarrow \text{product} \times 31 \times 33 \times 35 \times 37 \times \text{...} \times 57 \times 59 = 29{,}215{,}606{,}371{,}473{,}169{,}285{,}018{,}060{,}091{,}249{,}259{,}296{,}875`$<br/>
$`\text{result} \leftarrow \text{result} \times \text{product} = 115{,}476{,}893{,}502{,}183{,}682{,}653{,}166{,}335{,}352{,}659{,}171{,}719{,}555{,}028{,}600{,}718{,}376{,}458{,}740{,}234{,}375`$<br/>
29 values, $`3`$, $`5`$, $`7`$, $`9`$, $`11`$, $`13`$, $`15`$, $`17`$, $`19`$, ..., $`51`$, $`53`$, $`55`$, $`57`$ and $`59`$, get used up, with 16 multiplications done.

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
|1|2|<strike>3</strike>|4|<strike>5</strike>|<strike>6</strike>|<strike>7</strike>|8|<strike>9</strike>|<strike>10</strike>|<strike>11</strike>|<strike>12</strike>|<strike>13</strike>|<strike>14</strike>|<strike>15</strike>|16|<strike>17</strike>|<strike>18</strike>|<strike>19</strike>|<strike>20</strike>|
|<strike>21|<strike>22</strike>|<strike>23</strike>|<strike>24</strike>|<strike>25|<strike>26</strike>|<strike>27</strike>|<strike>28</strike>|<strike>29</strike>|<strike>30</strike>|<strike>31</strike>|32|<strike>33</strike>|<strike>34</strike>|<strike>35</strike>|<strike>36</strike>|<strike>37</strike>|<strike>38</strike>|<strike>39</strike>|<strike>40</strike>|
|<strike>41</strike>|<strike>42</strike>|<strike>43</strike>|<strike>44</strike>|<strike>45|<strike>46</strike>|<strike>47|<strike>48</strike>|<strike>49|<strike>50</strike>|<strike>51|<strike>52</strike>|<strike>53</strike>|<strike>54</strike>|<strike>55</strike>|<strike>56</strike>|<strike>57</strike>|<strike>58</strike>|<strike>59</strike>|<strike>60</strike>|

6. Finalizing:<br/>
The powers of 2 under $`n=60`$ account for 15 bits to shift ($`2^0`$, $`2^1`$, $`2^2`$, $`2^3`$, $`2^4`$, $`2^5`$).<br/>
During the calculation, we consumed:
   - When $`p`$ was $`4`$: $`1`$ value.
   - When $`p`$ was $`3`$: $`3`$ values.
   - When $`p`$ was $`2`$: $`7`$ values.
   - When $`p`$ was $`1`$: $`14`$ values.

&emsp;This makes a total of $`15+4 \times 1+3 \times 3 +2 \times 7 + 14 = 56`$ bits. Finally,
$`\text{result} \leftarrow \text{result} \times 2^{56}`$ by shifting bits.

$$
\begin{flalign}
& 60! = 8{,}320{,}987{,}112{,}741{,}390{,}144{,}276{,}341{,}183{,}223{,}364{,}380{,}754{,}172{,}606{,}361{,}245{,}952{,}449{,}277{,}696{,}409{,}600{,}000{,}000{,}000{,}000 &
\end{flalign}
$$

$`60`$ being relatively small, the number of multiplications used for the calculation ($`34`$) is still significantly higher than the $`n/2`$ ratio that the algorithm approaches.
We can see each successive loop gets more efficient than the previous one, though.
