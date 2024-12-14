
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Constants
#define MAX_LINES (400) // Maximum number of lines in the input file

// Stucts
typedef struct Machine
{
    int A_x;
    int A_y;
    int B_x;
    int B_y;

    long price_x;
    long price_y;
} Machine;

typedef enum PARSE_MODE
{
    PARSE_A,
    PARSE_B,
    PARSE_PRICE,
    PARSE_NEW_LINE,
} PARSE_MODE;

// Function prototypes

// Parses input from sdin
static void parse_input();

/** Calculates the cost of geting to the price
 * @param machine The machine to calculate the cost for
 * @return The cost of getting to the price -1 if it is not possible
 * @note If A and B are the same cost is calculated with brute force
 * @note Uses Cramers rule to solve the equation
 */
static long calculate_cost(Machine *machine);

/** Calculates the cost of geting to the price using brute force
 */
static long force_cost(Machine *machine);

/** Calculates the cost of geting to the price if the solution is on the line
 * @note New fuctions that is suposedly more efficient
 */
static long line_force_cost(Machine *machine);

// Global variables

// Number of machines
static int num_machines = 0;

// Array of machines
static Machine machines[MAX_LINES];

// Main functions

static int one_star()
{

    int total_cost = 0;

    for (int i = 0; i < num_machines; i++)
    {
        int cost = force_cost(&machines[i]);
        if (cost != -1)
        {

            total_cost += cost;
        }
    }
    return total_cost;
}

static unsigned long long two_star()
{
    // Begin by adding 10000000000000 to all price coordinates
    for (int i = 0; i < num_machines; i++)
    {
        machines[i].price_x += 10000000000000;
        machines[i].price_y += 10000000000000;
    }

    unsigned long long total_cost = 0;

    for (int i = 0; i < num_machines; i++)
    {

        long new_cost = calculate_cost(&machines[i]);

        if (new_cost != -1)
        {
            if (total_cost + new_cost < total_cost)
            {
                printf("Overflow\n");
                exit(EXIT_FAILURE);
            }
            total_cost += new_cost;
        }
    }
    return total_cost;
}

int main()
{
    parse_input();
    printf("One star: %d\n", one_star());
    printf("Two star: %llu\n", two_star());
    return 0;
}

// Function definitions

static void parse_input()
{
    char *line = NULL;
    size_t len = 0;
    size_t read;

    PARSE_MODE mode = PARSE_A;
    Machine *machine = &machines[0];

    while ((read = getline(&line, &len, stdin)) != -1)
    {
        switch (mode)
        {
        case PARSE_A:
            if (sscanf(line, "Button A: X+%d, Y+%d", &machine->A_x, &machine->A_y) != 2)
            {
                perror("Invalid input A");
                exit(EXIT_FAILURE);
            }
            mode = PARSE_B;
            break;
        case PARSE_B:
            if (sscanf(line, "Button B: X+%d, Y+%d", &machine->B_x, &machine->B_y) != 2)
            {
                perror("Invalid input B");
                exit(EXIT_FAILURE);
            }
            mode = PARSE_PRICE;
            break;
        case PARSE_PRICE:
            if (sscanf(line, "Prize: X=%ld, Y=%ld", &machine->price_x, &machine->price_y) != 2)
            {
                perror("Invalid input Price");
                exit(EXIT_FAILURE);
            }
            mode = PARSE_NEW_LINE;
            break;
        case PARSE_NEW_LINE:
            if (read != 1)
            {
                perror("Invalid input NEWLINE");

                exit(EXIT_FAILURE);
            }
            num_machines++;
            machine = &machines[num_machines];
            mode = PARSE_A;
            break;
        default:
            perror("Invalid parse mode");
            exit(EXIT_FAILURE);
        }
    }
    num_machines++;
    free(line);
}

long calculate_cost(Machine *m)
{
    /*

    | A_x A_y 1 | | x | = | price_X |
    | B_x B_y 1 | | y | = | price_y |

    */

    int deteminant = (m->A_x * m->B_y) - (m->A_y * m->B_x);

    // If A and B are the same cost is calculated with min(A, 3B)
    double A_s = (double)m->A_x / (double)m->A_y; // Slope of A
    double B_s = (double)m->B_x / (double)m->B_y; // Slope of B

    if (A_s == B_s)
    {
        /* 1. Check if price is on the line
           2. Calculate best cost if possible
        */
        double price_s = (double)m->price_x / (double)m->price_y; // Slope of price
        if (A_s != price_s)
        {
            printf("Price is not on the line\n");
            return -1;
        }
        return line_force_cost(m); // @todo REMOVE THIS AND FIX THE CODE
    }

    if (deteminant == 0) // No solution
    {
        return -1;
    }

    // Else use Cramers rule to solve the equation
    long a_step = (m->price_x * m->B_y - m->price_y * m->B_x); // A step in the equation
    long b_step = (m->A_x * m->price_y - m->A_y * m->price_x); // A step in the equation

    // Ensure A and B will be integers
    if (a_step % deteminant != 0 || b_step % deteminant != 0)
    {
        return -1;
    }

    long A = a_step / deteminant;
    long B = b_step / deteminant;

    return A * 3 + B;
}

long force_cost(Machine *m)
{
    int cost = INT_MAX;

    int A = 0, B = 0;

    int max_A = MIN(m->price_x / m->A_x, m->price_y / m->A_y);
    int max_B = MIN(m->price_x / m->B_x, m->price_y / m->B_y);

    for (A = 0; A <= max_A; A++)
    {
        for (B = 0; B <= max_B; B++)
        {
            if ((m->A_x * A + m->B_x * B) == m->price_x && (m->A_y * A + m->B_y * B) == m->price_y)
            {
                int new_cost = A * 3 + B;
                if (new_cost < cost)
                {
                    cost = new_cost;
                }
            }
            else if ((m->A_x * A + m->B_x * B) > m->price_x || (m->A_y * A + m->B_y * B) > m->price_y)
            {
                break;
            }
        }
        if ((m->A_x * A) > m->price_x || (m->A_y * A) > m->price_y)
        {
            break;
        }
    }
    if (cost == INT_MAX)
    {
        return -1;
    }

    return cost;
}

long line_force_cost(Machine *m)
{
    // Always on the line

    // There is always max two solutions THEORY?

    long A, B;

    long max_A = m->price_x / m->A_x; // Can use the fact that the slope is the same to only evaluate X
    long max_B = m->price_x / m->B_x;

    // Find cheapest option

    // since we know the slope is the same we can use only A

    if (m->A_x > m->B_x * 3)
    {
        // A is more efficient than B

        // A goes downwards from max_A
        for (A = max_A; A >= 0; A--)
        {
            // B goes upwards from 0
            for (B = 0; B <= max_B; B++)
            {
                if ((m->A_x * A + m->B_x * B) == m->price_x)
                {
                    return A * 3 + B; // If A was cheapest then the first combination that wors should be the cheapest
                }
                if ((m->A_x * A + m->B_x * B) > m->price_x)
                {
                    break;
                }
            }
        }
    }
    else
    {
        // B is more efficient than A

        // B goes downwards from max_B
        for (B = max_B; B >= 0; B--)
        {
            // A goes upwards from 0
            for (A = 0; A <= max_A; A++)
            {
                if ((m->A_x * A + m->B_x * B) == m->price_x)
                {
                    return A * 3 + B; // If A was cheapest then the first combination that wors should be the cheapest
                }
                if ((m->A_x * A + m->B_x * B) > m->price_x)
                {
                    break;
                }
            }
        }
    }
    return -1;
}
