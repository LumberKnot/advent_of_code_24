
#include <stdio.h>
#include <stdlib.h>

// Definitions

#define SIZE (50) // Change to 50 for the real input

// Structs

// Function prototypes

/* Parses input from std in and creates grid */
static void parse_input();

static void print_grid();

// Calculates the Greatest Common Divisor of two numbers
static int GCD(int a, int b);

// Global variables

// Input grid showing all antenas
unsigned char grid[SIZE][SIZE];

// Grid showing antinodes
unsigned char antinodes[SIZE][SIZE];

// Acounting for harmonics
unsigned char extended_antinodes[SIZE][SIZE];

// Main functions

/** Calculates total number of Antinodes
 *  @note Assumes grid is filled with input
 */
static int one_star()
{
    int num_antinodes = 0;

    // Only compares to antenas not yet evaluated
    int first_y, first_x, second_y, second_x;
    int x_dif, y_dif;

    int antinode_x, antinode_y;

    unsigned char antena;
    for (first_y = 0; first_y < SIZE; first_y++)
    {
        for (first_x = 0; first_x < SIZE; first_x++)
        {
            antena = grid[first_y][first_x];
            if (antena == '.')
            {
                continue;
            }

            // Here a new antena is found
            for (second_y = 0; second_y < SIZE; second_y++)
            {
                for (second_x = 0; second_x < SIZE; second_x++)
                {
                    if (first_y == second_y && first_x <= second_x)
                    {
                        continue; // Skip same antena or already evaluated
                    }
                    if (grid[second_y][second_x] != antena)
                    {
                        continue; // Skip different antena or none
                    }

                    /*Calculating Antinodes follows
                        x_dif = second_x - first_x;
                        y_dif = second_y - first_y;
                        Antinode_1 = (first_x - x_dif, first_y - y_dif)
                        Antinode_2 = (second_x + x_dif, second_y + y_dif)
                    */
                    x_dif = second_x - first_x;
                    y_dif = second_y - first_y;

                    // Antinode 1
                    antinode_x = first_x - x_dif;
                    antinode_y = first_y - y_dif;

                    if (antinode_x >= 0 && antinode_x < SIZE && antinode_y >= 0 && antinode_y < SIZE)
                    {
                        // within bounds
                        if (antinodes[antinode_y][antinode_x] == 0)
                        {
                            // not already an antinode
                            num_antinodes++;
                            antinodes[antinode_y][antinode_x] = 1;
                        }
                    }

                    // Antinode 2
                    antinode_x = second_x + x_dif;
                    antinode_y = second_y + y_dif;

                    if (antinode_x >= 0 && antinode_x < SIZE && antinode_y >= 0 && antinode_y < SIZE)
                    {
                        // within bounds
                        if (antinodes[antinode_y][antinode_x] == 0)
                        {
                            // not already an antinode
                            num_antinodes++;
                            antinodes[antinode_y][antinode_x] = 1;
                        }
                    }
                }
            }
        }
    }
    return num_antinodes;
}

/**Calculates total number of Antinodes including harmonics
 * @note Assumes grid is filled with input
 */
static int two_star()
{
    int num_antinodes = 0;

    // Only compares to antenas not yet evaluated
    int first_y, first_x, second_y, second_x;
    int x_dif, y_dif, gcd;

    int antinode_x, antinode_y;

    unsigned char antena;
    for (first_y = 0; first_y < SIZE; first_y++)
    {
        for (first_x = 0; first_x < SIZE; first_x++)
        {
            antena = grid[first_y][first_x];
            if (antena == '.')
            {
                continue;
            }

            // Here a new antena is found
            for (second_y = 0; second_y < SIZE; second_y++)
            {
                for (second_x = 0; second_x < SIZE; second_x++)
                {
                    if (first_y == second_y && first_x <= second_x)
                    {
                        continue; // Skip same antena or already evaluated
                    }
                    if (grid[second_y][second_x] != antena)
                    {
                        continue; // Skip different antena or none
                    }

                    // Time to calculate harmonics
                    x_dif = second_x - first_x;
                    y_dif = second_y - first_y;

                    gcd = GCD(x_dif, y_dif);
                    x_dif /= gcd;
                    y_dif /= gcd;

                    // Plus/minus 2*SIZE should cover all
                    for (int k = -2 * SIZE; k < 2 * SIZE; k++)
                    {
                        // Antinode 1
                        antinode_x = first_x + k * x_dif;
                        antinode_y = first_y + k * y_dif;
                        if (antinode_x >= 0 && antinode_x < SIZE && antinode_y >= 0 && antinode_y < SIZE)
                        {
                            // within bounds
                            if (extended_antinodes[antinode_y][antinode_x] == 0)
                            {
                                // not already an antinode
                                num_antinodes++;
                                extended_antinodes[antinode_y][antinode_x] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return num_antinodes;
}

int main(void)
{
    parse_input();
    printf("One star: %d\n", one_star());
    printf("Two star: %d\n", two_star());
    print_grid();
    return 0;
}

// Function definitions

static void parse_input()
{
    int c;
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            c = getchar();
            if (c == EOF)
            {
                perror("Unexpected EOF");
                exit(EXIT_FAILURE);
            }
            grid[y][x] = (unsigned char)c;
            antinodes[y][x] = 0;
            extended_antinodes[y][x] = 0;
        }
        getchar(); // Skip newline
    }
}

void print_grid()
{
    unsigned char c;
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {

            c = grid[y][x];
            if (c == '.' && extended_antinodes[y][x] == 1)
            {
                printf("#");
            }
            else
            {
                printf("%c", grid[y][x]);
            }
        }
        printf("\n");
    }
}

int GCD(int a, int b)
{
    // Minimum of two numbers
    int min = (abs(a) < abs(b)) ? abs(a) : abs(b);
    for (int i = min; i > 0; i--)
    {
        if (a % i == 0 && b % i == 0)
        {
            return i;
        }
    }

    printf("No GCD found %d %d\n", a, b);
    exit(EXIT_FAILURE);
}
