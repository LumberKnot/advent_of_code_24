
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structs

typedef struct map_node
{
    unsigned long long value; // Value writen on the stone
    unsigned long amount;     // Amount of stones with the same value
    struct map_node *next;
} map_node;

// Fuction prototypes

static void add_to_map(map_node **map, unsigned long long value, unsigned long amount);

static int digits(unsigned long long number);

/** Solves number of stones in total after steps steps
 * @param stones Map of stones with their values and amounts
 * @param steps Number of steps left
 */
static unsigned long long solve(map_node *stones, int steps);

// Global variables

const unsigned long long input[8] = {
    0,
    4,
    93,
    314,
    1304,
    54276,
    664481,
    9694820};

// Main functions

static unsigned long long one_star()
{
    map_node *stones = NULL;

    for (int i = 0; i < 8; i++)
    {
        add_to_map(&stones, input[i], 1);
    }

    return (solve(stones, 25));
}

static unsigned long long two_star()
{
    map_node *stones = NULL;

    for (int i = 0; i < 8; i++)
    {
        add_to_map(&stones, input[i], 1);
    }

    return (solve(stones, 75));
}

int main()
{
    printf("One star: %llu\n", one_star());
    printf("Two star: %llu\n", two_star());
    return 0;
}

// Function definitions

static void add_to_map(map_node **map, unsigned long long value, unsigned long amount)
{
    // First see if the value is already in the map
    map_node *current = *map;

    while (current != NULL)
    {
        if (current->value == value)
        {
            current->amount += amount;
            return;
        }
        current = current->next;
    }

    // else add it to the map
    map_node *new_node = malloc(sizeof(map_node));
    new_node->value = value;
    new_node->amount = amount;
    new_node->next = *map;
    *map = new_node;
}

static int digits(unsigned long long number)
{
    int digits = 0;
    while (number)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

static unsigned long long solve(map_node *stones, int steps)
{
    map_node *current = stones;
    map_node *temp;
    if (steps == 0)
    {
        unsigned long long total = 0;
        while (current != NULL)
        {
            total += current->amount;

            temp = current;
            current = current->next;
            free(temp);
        }
        return total;
    }

    map_node *new_map = NULL;

    // For each stone calculate the next steps
    unsigned long long total = 0;

    unsigned long long value;
    unsigned long amount;

    while (current != NULL)
    {
        value = current->value;
        amount = current->amount;
        temp = current;
        current = current->next;
        free(temp);
        temp = NULL;

        // Rule 1
        if (value == 0)
        {
            add_to_map(&new_map, 1, amount);
            continue;
        }

        int dig = digits(value);
        int power = pow(10, dig / 2);

        // Rule 2
        if (dig % 2 == 0)
        {
            add_to_map(&new_map, value / power, amount);
            add_to_map(&new_map, value % power, amount);
            continue;
        }

        // Rule 3
        add_to_map(&new_map, value * 2024, amount);
    }

    return solve(new_map, steps - 1);
}
