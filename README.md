# Advent of code 2024 in c
My attempt at solving advent of code in c. 

## Progress

| Day|       |Day||
| -- | ----- | - | - |
| 01 | :star: :star: |

## Days
### Day 1
Noticed how easy some other languages make problems like this. In scala which I am most familiar with the whole operation for the first star can be writen in one line asuming you have the input in the vectors left and right:
```scala
left.sorted
    .zip(right.sorted)
    .map((l,r) => Math.abs(l-r))
    .sum
```
