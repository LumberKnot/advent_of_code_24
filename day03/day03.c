#include <stdio.h>
#include <stdlib.h>

#define ONE_STAR_LINES 694
#define TWO_STAR_LINES 350

/** Using grep outside of the program to filter all input
 *  grep -o -P 'mul\(\d+,\d+\)' input > input_one_star
 *  @note Knows the number of lines to be 694
 *  @note Input is passed through stdin
 */
long one_star()
{
    long total = 0;
    for (int i = 0; i < ONE_STAR_LINES; i++)
    {
        int a, b;
        scanf("mul(%d,%d)\n", &a, &b);
        total += a * b;
    }
    return total;
}

/** Also using grep to find all relevant ocurences
 * grep -P -o "mul\(\d+,\d+\)|do\(\)|don't\(\)" input | awk 'BEGIN {in_block=1} /do\(\)/ {in_block=1} /don/ {in_block=0} in_block' | grep 'mul' > input_two_star
 * @note Input is passed through stdin
 * @note
 */
long two_star()
{
    long total = 0;
    for (int i = 0; i < TWO_STAR_LINES; i++)
    {
        int a, b;
        scanf("mul(%d,%d)\n", &a, &b);
        total += a * b;
    }
    return total;
}

int main()
{
    // printf("One star: %ld\n", one_star());
    printf("Two star: %ld\n", two_star());
    return 0;
}