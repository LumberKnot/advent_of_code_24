# Advent of code 2024 in c
My attempt at solving advent of code in c. 

## Progress

| Day|       |Day||
| -- | ----- | - | - |
| 01 | :star: :star: | 02 | :star: :star: |

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