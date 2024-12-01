#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_LINES (1000)

/** Insertion sorting an element
 * @param number_of_elements the number of elements already in the array
 */
void insert(int *arr, int value, int number_of_elements);

/** Counts the number of apearences of value in the array
 * @param arr the array to search
 * @param value the value to search for
 * @return the number of ocurences
 * @note the array is assumed to have NUMBER_OF_LINES elements
 */
int ocurences(int *arr, int value);

static int *left_side;
static int *right_side;

static int fill_arrays()
{
    left_side = malloc(NUMBER_OF_LINES * sizeof(int));
    right_side = malloc(NUMBER_OF_LINES * sizeof(int));
    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        scanf("%d %d", &left_side[i], &right_side[i]);
    }
    return 0;
}

long one_star()
{
    // Arays to store the sorted input
    int *left_side_sorted = malloc(NUMBER_OF_LINES * sizeof(int));
    int *right_side_sorted = malloc(NUMBER_OF_LINES * sizeof(int));
    int A, B;
    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        A = left_side[i];
        B = right_side[i];
        insert(left_side_sorted, A, i);
        insert(right_side_sorted, B, i);
    }

    long total_diff = 0;
    // Sum the differences for each index
    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        total_diff += abs(left_side_sorted[i] - right_side_sorted[i]);
    }
    free(left_side_sorted);
    free(right_side_sorted);
    return total_diff;
}

/** Needs to calculate the sum of A_i * B(A_i)
 *  meaning the sum of all elements in A multiplied by the number of times they appear in B
 *  My solution has a time complexity of O(n^2) and a space complexity of O(n)
 */
long two_star()
{

    long total = 0;
    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        total += left_side[i] * ocurences(right_side, left_side[i]);
    }
    return total;
}

int main()
{
    fill_arrays();
    printf("One star result %ld\n", one_star());
    printf("Two star result %ld\n", two_star());
    free(left_side);
    free(right_side);
    return 0;
}

//@todo error is here i think

void insert(int *arr, int value, int number_of_elements)
{
    int i = 0;
    while (i < number_of_elements && arr[i] < value)
    {
        i++;
    }
    for (int j = NUMBER_OF_LINES - 1; j > i; j--)
    {
        arr[j] = arr[j - 1];
    }
    arr[i] = value;
}

int ocurences(int *arr, int value)
{
    int total = 0;
    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        if (arr[i] == value)
        {
            total++;
        }
    }
    return total;
}
