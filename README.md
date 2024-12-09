# Advent of code 2024 in c
My attempt at solving advent of code in c. 

## Progress


| 01 | :star: :star: | 02 | :star: :star: | 03 | :star: :star: |
| -- | ----- | - | - | - | - |
| **04** | :star: :star: |**05**| :star: :star: |**06**| :star: :star:|
| **07** | :star: :star: | **08** | :star: :star: |||

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