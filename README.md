# Advent of code 2024 in c
My attempt at solving advent of code in c. 

## Progress


| **01** | :star: :star: | **02** | :star: :star: | **03** | :star: :star: | **04** | :star: :star: | **05** | :star: :star: |
| ------ | ------------- | ------ | ------------- | ------ | ------------- | ------ | ------------- | ------ | ------------- |
| **06** | :star: :star: | **07** | :star: :star: | **08** | :star: :star: | **09** | :star: :star: | **10** | :star: :star: |
| **11** | :star: :star: | **12** | :star:        | **13** | :star: :star: | **14** | :star: :star: | **15** | :star:        |
| **16** | :star:        | **17** | :star: :star: | **18** | :star: :star: | **19** |               | **20** | :star:        |
| **21** |               | **22** | :star: :star: | **23** | :star: :star: | **24** |               | **25** | :star:        |

## Days
### Day 1
Noticed how easy some other languages make problems like this. In scala which I am most familiar with the whole operation for the first star can be writen in one line asuming you have the input in the vectors left and right:
```scala
left.sorted
    .zip(right.sorted)
    .map((l,r) => Math.abs(l-r))
    .sum
```
### Day 2
Once again noticed how easy it would be to do in scala. The second star was solved very unelegantly, with a complexity of O(m*n^2^) with m records and n levels. In essense I performed the same safe check as in the first star for the whole list and every sublist missing exactly one element.

### Day 3
This was solved mostly though the `grep` command in linux. I realised during my first attempt of solving the first star that I was essentily implementing `grep` and decided to just use it.

The first star was solved almost entirely by this command
> grep -o -P 'mul\\(\d+,\d+\\)' input > input_one_star

The second required a more complicated command to filter out everythin in between do:s and don't:s
> grep -P -o "mul\\(\d+,\d+\\)|do\\(\)|don't\(\)" input | awk 'BEGIN {in_block=1} /do\(\)/ {in_block=1} /don/ {in_block=0} in_block' | grep 'mul' > input_two_star

### Day 4
Nothing interersting really.

### Day 6
Solved this before day 5 because it seemed more interersting. Solving both stars revolved around storing information of the matrix that represented the map. I created a bitmask (0b1000, 0b0100 ...) that represenented the directions. If a square had 0 you added one  to the total number of steps and if it already had the bitmask that you were going to write you had found a loop.

### Day 5
Took a long time to get the second star as I faultily asumed the problem would have been formulated in a way that every update only had exactly one working solution so I tried to do a version of iterative bubble sorting that ended in infinite loops

### Day 7
Not super interesting. Testing every calculation by looping over all the posible combinations of + * ||. 

### Day 8
Two nice equations appear in this problem. To solve the first star you need to realize that antinodes will always apear att A~1~ - (A~2~ - A~1~) and A~2~ + (A~2~ - A~1~) if A~i~ is antena number i in the pair. For the second start it is vital to remember to divide with the GCD before creating the line

### Day 10
Did this day before 9 because I was very early on it. I solved this using recursion and actually felt that the first star was harder than the second. The first star required you to know if you had visited an endpoint before while the first did not care.

### Day 9
First star was solved with two pointers that traversed the memory from either side. When the leading pointer was on an empty slot the trailing pointer moved until it found an non empty slot that was then multiplied with the index it would end up on.

Second star was solved with a second representation of the memory that only tracked the size of each empty block.

### Day 14
First star was simply stepping trough 100 steps. I found it was easiets not to make a grid but let each robot maintain all of its own information. For the second star I printed every step to a file and searched through it with ctl f.

### Day 13
First star could easily be solved with brute force. The second star had two kinks. All claw machines where the slope for A and B were diferent could be solved with Cramers rule. If they had the same slope and the price laid on that line it was calculated with brute force but starting with the max number of the longest claw change of A and 3*B 

### Day 18
First star was solved with a simple bfs search and the second star was done by applying all corruptions and then removing them backwards one by one untill a solution was posible.

### Day 22
First star was just a matter of doing, second as usual was far more fun. I solved it with two four dimensional *matrices*. Both were 19\*19\*19\*19 as that is the number of posible differences that exist between two single digit numbers and represent a sequence. First was a global exchange rate: number of bananas for a given sequence and other was local for each monkey that simply keept track if the sequence had already been visited (only first visit was valuable).

### Day 23
First star was made by first creating all 3 pairs in growing node order and then filter out all that start with *t* to avoid duplicates. Second was solved recursevly also following order

### Day 11
Solved best using a table of stones so that every stone of the same value can be calcultated simultaneusly.