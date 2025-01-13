
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Longest towel is is 8 stripes
 *  All towels are represented as their string
 * Longest pattern is 60 stripes
 *
 * Uses a slightly modified input to make it easier to read
 */

#define MAX_PATTERNS (500)      // Arbitrary large enough number
#define MAX_TOWELS (500)        // Arbitrary large enough number
#define MAX_PATTERN_LENGTH (60) // 60 stripes
#define MAX_TOWEL_LENGTH (9)    // 8 stripes + 1 for null terminator

#define MAP_SIZE (7919) // Prime number
#define NOT_FOUND (-1)  // Really hope this does not occur in the hash

#define RED (0b001)
#define GREEN (0b010)
#define BLUE (0b100)
#define BLACK (0b110)
#define WHITE (0b111)

// Structs

typedef struct map_node map_node;

typedef map_node **map; // Array of map_nodes

struct map_node
{
    char *key;
    long long value;
    map_node *next;
};

// Global variables

/* All towels are stored in this matrix, the intex is the first color, 0 is therefore not used*/
static char **patterns;
static char **towels;
static int number_of_patterns = 0;
static int number_of_towels = 0;

// Function prototypes

static long long solve_pattern(map map, const char *pattern);

static void parse_input();

static void free_memory();

// Map functions

static map map_create();

static void map_insert(map map, const char *key, long long value);

/** Returns the value of the key, or NOT_FOUND if not found
 *
 */
static long long map_get(map map, const char *key);

static void map_free(map map);

static inline unsigned long quick_hash(const char *key);

#ifdef DEBUG
static void debug_map(map map);
#endif

// Array functions

/** Inserts value by reverse string lenght
 * @note Longest string will be first
 * @param array Array to insert into
 * @param value Value to insert
 * @param array_length Numbers of elements in array before insert
 */
static void array_insert(char **array, char *value, int array_length);

// Main functions

int main()
{
    parse_input();

    long num_possible = 0;
    long long total_solutions = 0;

    map map = map_create();

    // solve_pattern(map, "grwggrggbrurwugurbggwrguurrubwruugwrruubg"); // problematic pattern

    for (int i = number_of_patterns - 1; i >= 0; i--)
    {
        long long solution = solve_pattern(map, patterns[i]);
        if (solution)
        {
            num_possible++;
            total_solutions += solution;
        }
    }

    printf("Number of patterns that are possible: %ld\n", num_possible);
    printf("Total number of solutions: %llu\n", total_solutions);

#ifdef DEBUG
    debug_map(map);
#endif

    map_free(map);

    free_memory();

    return 0;
}

// Function definitions

long long solve_pattern(map map, const char *pattern)
{

    long long solutions = 0;

    if (*pattern == '\0')
    {
        // Reached the end of the pattern
        return 1;
    }

    // Check map for pattern
    long long value = map_get(map, pattern);

    if (value != NOT_FOUND)
    {
        return value;
    }

    // Otherwise we need to try all possible towels
    for (int towel_id = 0; towel_id < number_of_towels; towel_id++)
    {
        // Check first stripe
        if (*towels[towel_id] != *pattern)
        {
            continue;
        }

        size_t towel_length = strlen(towels[towel_id]);
        if (strncmp(towels[towel_id], pattern, towel_length))
        {
            // Does not match
            continue;
        }

        solutions += solve_pattern(map, pattern + towel_length);
    }

    // Add to map
    map_insert(map, pattern, solutions);

    return solutions;
}

void parse_input()
{
    towels = malloc(MAX_TOWELS * sizeof(char *));
    patterns = malloc(MAX_PATTERNS * sizeof(char *));

    // First line is towels available

    towels[0] = malloc(MAX_TOWEL_LENGTH * sizeof(char));

    char *line = malloc(MAX_TOWEL_LENGTH * MAX_TOWELS * sizeof(char));
    char *towels_free = line;
    char *runner;

    scanf("%s", line);

    while (*line != '\0')
    {
        runner = strpbrk(line, ",");
        strtok(line, ","); // Split string
        array_insert(towels, line, number_of_towels++);
        line = runner + 1;
    }

    free(towels[number_of_towels]);

    // Second line is number of patterns

    while (scanf("%s", line) != EOF)
    {
        array_insert(patterns, line, number_of_patterns++);
    }

    free(towels_free);
}

void free_memory()
{
    for (int i = 0; i < number_of_towels; i++)
    {
        free(towels[i]);
    }

    for (int i = 0; i < number_of_patterns; i++)
    {
        free(patterns[i]);
    }

    free(towels);
    free(patterns);
}

// Map function definitions

map map_create()
{
    map m = calloc(MAP_SIZE, sizeof(map_node *));
    if (m == NULL)
    {
        fprintf(stderr, "Could not allocate memory for map\n");
        exit(1);
    }

    return m;
}

void map_insert(map map, const char *key, long long value)
{
    if (value == NOT_FOUND)
    {
        perror("Value is NOT_FOUND");
        exit(1);
    }

    unsigned long hash = quick_hash(key);

    map_node *node = map[hash];

    if (node == NULL)
    {
        node = malloc(sizeof(map_node));
        map[hash] = node;
    }
    else
    {
        while (node->next != NULL)
        {
            node = node->next;
        }
        node->next = malloc(sizeof(map_node));
        node = node->next;
    }

    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->value = value;
    node->next = NULL;
}

long long map_get(map map, const char *key)
{
    unsigned long hash = quick_hash(key);

    map_node *node = map[hash];

    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }

    return NOT_FOUND;
}

void map_free(map map)
{
    map_node *temp, *node;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        node = map[i];
        while (node != NULL)
        {
            temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map);
}

inline unsigned long quick_hash(const char *key)
{
    unsigned long hash = 0;

    long i = 0;            // Always modulo sizeof(long) * 8;
    const long i_max = 13; // resonable number to fit into 7919

    long c;

    while (*key != '\0')
    {
        switch (*key)
        {
        case 'r':
            c = RED;
            break;
        case 'g':
            c = GREEN;
            break;
        case 'u':
            c = BLUE;
            break;
        case 'b':
            c = BLACK;
            break;
        case 'w':
            c = WHITE;
            break;
        default:
            perror("Invalid color");
            exit(1);
        }

        hash ^= c << (i);
        i = (i + 3) % i_max;
        key++;
    }

    return hash % MAP_SIZE;
}

#ifdef DEBUG
void debug_map(map map)
{
    long slots = 0;         // Number of used slots in map
    long elements = 0;      // Number of elements in map
    long collisions = 0;    // Number of slots with more than one element * number of elements in slot
    long longest_chain = 0; // Longest chain in map
    long chain_length = 0;  // Current chain length

    map_node *node;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        node = map[i];

        if (node == NULL)
        {
            continue;
        }

        elements++;
        slots++;

        if (node->next != NULL)
        {
            node = node->next;
            collisions++;
            chain_length = 1;
            while (node != NULL)
            {
                chain_length++;
                collisions++;
                elements++;
                node = node->next;
            }

            if (chain_length > longest_chain)
            {
                longest_chain = chain_length;
            }
        }
    }

    printf("Slots:                  %ld\n", slots);
    printf("Elements:               %ld\n", elements);
    printf("Collisions:             %ld\n", collisions);
    printf("Longest chain:          %ld\n\n", longest_chain);

    printf("Fullness:               %f%%\n", (double)slots / MAP_SIZE * 100.0);
    printf("Load factor:            %f\n", (double)elements / MAP_SIZE);
    printf("Collisions per element: %f\n", (double)collisions / elements);
}
#endif

void array_insert(char **array, char *value, int array_length)
{
    size_t len = strlen(value);
    int i = 0;

    while (i < array_length && len < strlen(array[i]))
    {
        i++;
    }

    if (i != array_length)
    {
        for (int j = array_length + 1; j > i; j--)
        {
            array[j] = array[j - 1];
        }
    }

    array[i] = malloc(len + 1);
    strcpy(array[i], value);
}