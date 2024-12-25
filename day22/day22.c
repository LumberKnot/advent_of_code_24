
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIX(x, secret) ((x) ^ secret)
#define PRUNE(x) ((x) % 16777216)

#ifndef DEBUG
#define NUM_MONKEYS (1767) // Exact number of monkeys from input
#else
#define NUM_MONKEYS (4) // Exact number of monkeys from input
#endif
#define NUM_STEPS (2000) // Number of steps in the algorithm

// Function prototypes

static void parse_input();

/** Not sure if secret actualy is the input */
static unsigned long next(unsigned long secret);

// Global variables

static unsigned long *secrets; // List of all monkeys secret numbers

// Main functions

static unsigned long one_star()
{
    unsigned long new_secret;
    for (int step = 1; step <= NUM_STEPS; step++)
    {
        for (int monkey_index = 0; monkey_index < NUM_MONKEYS; monkey_index++)
        {
            new_secret = next(secrets[monkey_index]);
            secrets[monkey_index] = new_secret;
        }
    }

    unsigned long long sum = 0;
    for (int i = 0; i < NUM_MONKEYS; i++)
    {
        sum += secrets[i];
    }
    return sum;
}

static unsigned int two_star()
{

    /** Global exange matrix (4d)
     * Shows the total number of bananas you can get for a sequence of four differences
     * Differences can be between -9 and 9 (inclusive) so index is calculated as 9 + difference
     * First dimension is first difference in sequence, fourth is is last difference in sequence
     */
    unsigned int global_exchange[19][19][19][19];

    /** Local exchange matrix (4d) Any non 0 value means sequence has already been visited and cannot be revisited
     * Shows the total number of bananas you can get for a sequence of four differences
     * Differences can be between -9 and 9 (inclusive) so index is calculated as 9 + difference
     * First dimension is first difference in sequence, fourth is is last difference in sequence
     */
    unsigned int local_exchange[19][19][19][19];

    // The exchange array for one monkey
    unsigned char *exchange = malloc(sizeof(char) * (NUM_STEPS)); // Cannot have difference before first step

    int d1, d2, d3, d4;
    unsigned long new_secret;
    int leading_edge;

    // Zero out global exchange matrix
    for (d1 = 0; d1 < 19; d1++)
    {
        for (d2 = 0; d2 < 19; d2++)
        {
            for (d3 = 0; d3 < 19; d3++)
            {
                for (d4 = 0; d4 < 19; d4++)
                {
                    global_exchange[d1][d2][d3][d4] = 0;
                }
            }
        }
    }

    // For each monkey
    for (int monkey_index = 0; monkey_index < NUM_MONKEYS; monkey_index++)
    {
        // Zero out local exchange matrix
        for (d1 = 0; d1 < 19; d1++)
        {
            for (d2 = 0; d2 < 19; d2++)
            {
                for (d3 = 0; d3 < 19; d3++)
                {
                    for (d4 = 0; d4 < 19; d4++)
                    {
                        local_exchange[d1][d2][d3][d4] = 0;
                    }
                }
            }
        }

        // Add first step to exchange array
        exchange[0] = secrets[monkey_index] % 10;

        // For each step
        for (int step = 1; step <= NUM_STEPS; step++)
        {
            new_secret = next(secrets[monkey_index]);
            exchange[step] = (new_secret % 10); // Add exchange rate to exchange array
            secrets[monkey_index] = new_secret;
        }

        // Time to check sequences
        for (leading_edge = 1; leading_edge < NUM_STEPS - 3; leading_edge++) // 3 steps lookahead
        {
            d1 = 9 + exchange[leading_edge] - exchange[leading_edge - 1];     // Calculate index for first difference
            d2 = 9 + exchange[leading_edge + 1] - exchange[leading_edge];     // Calculate index for second difference
            d3 = 9 + exchange[leading_edge + 2] - exchange[leading_edge + 1]; // Calculate index for third difference
            d4 = 9 + exchange[leading_edge + 3] - exchange[leading_edge + 2]; // Calculate index for fourth difference

            if (local_exchange[d1][d2][d3][d4] == 0) // If sequence has not been visited
            {
                local_exchange[d1][d2][d3][d4] = 1;                            // Mark sequence as visited
                global_exchange[d1][d2][d3][d4] += exchange[leading_edge + 3]; // Increase global exchange for sequence
            }
        }
    }

    // Find the best sequence
    unsigned int best_exchange = 0;
    int best_d1 = -1, best_d2 = -1, best_d3 = -1, best_d4 = -1;
    for (d1 = 0; d1 < 19; d1++)
    {
        for (d2 = 0; d2 < 19; d2++)
        {
            for (d3 = 0; d3 < 19; d3++)
            {
                for (d4 = 0; d4 < 19; d4++)
                {
                    if (global_exchange[d1][d2][d3][d4] > best_exchange)
                    {
                        best_exchange = global_exchange[d1][d2][d3][d4];
                        best_d1 = d1;
                        best_d2 = d2;
                        best_d3 = d3;
                        best_d4 = d4;
                    }
                }
            }
        }
    }
    printf("Best exchange: %u at %d %d %d %d\n", best_exchange, best_d1 - 9, best_d2 - 9, best_d3 - 9, best_d4 - 9);
    return best_exchange;
}
int main()
{
    parse_input();
    printf("One star: %lu\n", one_star());
    printf("Two star: %u\n", two_star());
    free(secrets);
}

// Function definitions

static void parse_input()
{
    secrets = malloc(sizeof(unsigned long) * NUM_MONKEYS);

    for (int i = 0; i < NUM_MONKEYS; i++)
    {
        scanf("%lu", &secrets[i]);
    }
}

static unsigned long next(unsigned long secret)
{
    unsigned long result;
    result = secret * 64; // Step one

    secret = MIX(result, secret); // mix step
    secret = PRUNE(secret);       // prune step

    result = secret / 32; // Step two (integer division)

    secret = MIX(result, secret); // mix step
    secret = PRUNE(secret);       // prune step

    result = secret * 2048;       // Step three
    secret = MIX(result, secret); // mix step
    secret = PRUNE(secret);       // prune step

    return secret;
}
