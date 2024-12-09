
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Constants
#define MAX_CALCULATIONS (850) // Based on the input
#define MAX_STACK_SIZE (12)    // Based on the input

#define CALCULATION_PLUS (0b00)
#define CALCULATION_MULT (0b01)
#define CALCULATION_CONC (0b10)
#define CALCULATION_NONE (0b11)

// Structure definitions

/** Holds the information of an ongoing calculation.
 */
typedef struct calculation_t
{
    long answer;    /**< The expected answer of the calculation. */
    long *stack;    /**< Pointer to an array holding all the numbers from leftmost (index 0) to rightmost (index stack_size-1). */
    int stack_size; /**< The size of the stack array. */
    int simple;     /**< If the calculation is simpl (only + and *). */
} calculation_t;

// Function definitions

/** Parses input and creates calculations */
static void parse_input();

/** Frees all calculations */
static void free_calculations();

/** Calculates if an answer is posible to create
 * @param calculation The calculation to check
 * @return anwer if possible, 0 otherwise
 */
static long calculate_plausible_simple(calculation_t *calculation);

/** Calculates if an answer is posible to create using + * and ||
 *
 */
static long calculate_plausible_advanced(calculation_t *calculation);

/* Counts number of digits in a number */
static int digits(long number);

/** Checks if a way is viable
 * @param way The way to check
 * @param num_operations The number of operations in the way
 * @return 1 if viable, 0 otherwise
 * @note A way is viable if it does not contain any NONE operations
 */
int isViable(int way, int num_operations);

// Global variables

static calculation_t *calculations; // All calculations
static int number_of_calculations = 0;

// Main program

// Calculates the summ of all aswers that can be computed from the given stack using + *
static long one_star()
{
    long sum = 0;
    for (int i = 0; i < number_of_calculations; i++)
    {
        sum += calculate_plausible_simple(&calculations[i]);
    }
    return sum;
}

// Calculates the sum of all answers that can be computed from the given stack using + * and || (concat)
static long two_star()
{
    long sum = 0;
    for (int i = 0; i < number_of_calculations; i++)
    {
        if (calculations[i].simple)
        {
            // Do not need to recalculate
            sum += calculations[i].answer;
        }
        else
        {
            sum += calculate_plausible_advanced(&calculations[i]);
        }
    }
    return sum;
}

int main()
{
    parse_input();

    printf("One star: %ld\n", one_star());
    printf("Two star: %ld\n", two_star());

    return 0;
}

// Function implementations

static void parse_input()
{
    calculations = malloc(MAX_CALCULATIONS * sizeof(calculation_t));

    char *line = NULL;
    char *pointer; // Moves along line

    size_t len = 0;
    size_t bytes_read;
    int calculation_index = 0;

    while ((bytes_read = getline(&line, &len, stdin)) != -1)
    {
        /* All calculations are on the form
            answer: number number number ...
            where answer is the expected answer of the calculation
        */

        pointer = line;

        line[bytes_read - 1] = ' '; // Make the newline a space to simplify parsing

        long answer;
        long *stack = malloc(MAX_STACK_SIZE * sizeof(long));
        int stack_size = 0;

        sscanf(line, "%ld:", &answer);
        pointer = strchr(line, ':') + 2; // Move pointer to the first number

        while (sscanf(pointer, "%ld", &stack[stack_size]) == 1)
        {
            stack_size++;
            pointer = strchr(pointer, ' ') + 1; // Move pointer to the next number
        }

        calculations[calculation_index].answer = answer;
        calculations[calculation_index].stack = stack;
        calculations[calculation_index].stack_size = stack_size;
        calculations[calculation_index].simple = 0; // All calculations are assumed to be advanced
        calculation_index++;
    }
    number_of_calculations = calculation_index;
}

void free_calculations()
{
    for (int i = 0; i < number_of_calculations; i++)
    {
        free(calculations[i].stack);
    }
    free(calculations);
}

static long calculate_plausible_simple(calculation_t *calculation)
{
    // There are 2^(stack_size-1) possible ways to insert + and * in the stack
    // We can represent this as a binary number where 0 means + and 1 means *

    int number_of_ways = 1 << (calculation->stack_size - 1);

    long *inner_stack = malloc(calculation->stack_size * sizeof(long)); // Used here to not alter the calculation stack

    for (int way = 0; way < number_of_ways; way++)
    {
        int operation_tracker = way; // Used to track the current operation, LSB is the leftmost operation

        // copy the stack
        for (int j = 0; j < calculation->stack_size; j++)
        {
            inner_stack[j] = calculation->stack[j];
        }
        int operation_index = 0; // Starts at the leftmost operation (is actually a pointer to the number)

        while (operation_index < calculation->stack_size - 1)
        {
            if ((operation_tracker & 0b1) == 0)
            {
                // Summation +
                inner_stack[operation_index + 1] += inner_stack[operation_index];
            }
            else
            {
                // Multiplication *
                inner_stack[operation_index + 1] *= inner_stack[operation_index];
            }
            operation_index++;
            operation_tracker >>= 1; // Move to the next operation
        }

        if (inner_stack[calculation->stack_size - 1] == calculation->answer)
        {
            free(inner_stack);
            calculation->simple = 1;
            return calculation->answer;
        }
    }

    free(inner_stack);
    calculation->simple = 0;
    return 0;
}

static long calculate_plausible_advanced(calculation_t *calculation)
{
    // There are 4^(stack_size-1) possible ways to insert +, *, and || in the stack (technically 3^(stack_size-1))
    // We can represent this as a binary number where 00 means +, 01 means *, 10 means || and 11 means nothing

    int number_of_ways = 1 << (2 * (calculation->stack_size - 1)); // 2 bits per operation

    long *inner_stack = malloc(calculation->stack_size * sizeof(long)); // Used here to not alter the calculation stack

    for (int way = 0; way < number_of_ways; way++)
    {
        int operation_index; // Starts at the leftmost operation (is actually a pointer to the number)

        // Asses viability of way (should include no NONEs)
        if (!isViable(way, calculation->stack_size - 1))
        {
            continue;
        }

        int operation_tracker = way; // Used to track the current operation, LSB is the leftmost operation
        operation_index = 0;         // Reset operation_index

        // copy the stack
        for (int j = 0; j < calculation->stack_size; j++)
        {
            inner_stack[j] = calculation->stack[j];
        }

        while (operation_index < calculation->stack_size - 1)
        {
            int operation = (operation_tracker & 0b11);
            switch (operation)
            {
            case CALCULATION_PLUS:
                // Summation +
                inner_stack[operation_index + 1] += inner_stack[operation_index];
                break;
            case CALCULATION_MULT:
                // Multiplication *
                inner_stack[operation_index + 1] *= inner_stack[operation_index];
                break;
            case CALCULATION_CONC:
                // Concatenation ||
                long scale = (long)pow(10.0, digits(inner_stack[operation_index + 1]));
                inner_stack[operation_index + 1] = inner_stack[operation_index] * scale + inner_stack[operation_index + 1];
                break;
            case CALCULATION_NONE:
                perror("NONE operation passed filter");
                exit(EXIT_FAILURE);
                break;
            default:
                perror("Unknown operation");
                exit(EXIT_FAILURE);
                break;
            }
            operation_index++;
            operation_tracker >>= 2; // Move to the next operation
        }

        if (inner_stack[calculation->stack_size - 1] == calculation->answer)
        {
            free(inner_stack);
            return calculation->answer;
        }
    }

    free(inner_stack);
    return 0;
}

int digits(long number)
{
    int count = 0;
    while (number != 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

int isViable(int way, int num_operations)
{
    int operation_index;
    for (operation_index = 0; operation_index < num_operations; operation_index++)
    {
        if (((way >> (2 * operation_index)) & 0b11) == 0b11)
        {
            return 0;
        }
    }
    return 1;
}
