# Advent of code 2024 in c
My attempt at solving advent of code in c. 

## Progress


| 01 | :star: :star: | 02 | :star: :star: | 03 | :star: :star: |
| -- | ----- | - | - | - | - |
| **04** | :star: :star: |||||

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