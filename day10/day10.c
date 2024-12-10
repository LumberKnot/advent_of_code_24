
#include <stdio.h>
#include <stdlib.h>

#define SIZE (42)        // Square is 42x42
#define TRAILHEADS (194) // Number of trailheads, from my input
#define TRAIL_ENDS (142) // Number of trail ends, from my input

#define TEMP (-1)

typedef struct trailhead_list_t trailhead_list_t;

// Type definitions
typedef struct trailhead_t
{
    int x;
    int y;
} trailhead_t;

struct trailhead_list_t
{
    trailhead_t trailhead;
    trailhead_list_t *next;
};

// Global variables
static char map[SIZE][SIZE];
static trailhead_list_t *trailhead_list = NULL;

// Function prototypes

void parse_input();

int number_of_trails(int x, int y, char value);
int number_of_trails_extended(int x, int y, char value);

void restore_map();

// Main function

int one_star()
{
    int total = 0;
    trailhead_list_t *tl = trailhead_list;

    if (tl == NULL)
    {
        perror("No trailheads found");
        exit(1);
    }

    while (tl != NULL)
    {
        int num_trails = number_of_trails(tl->trailhead.x, tl->trailhead.y, 0);
        total += num_trails;
        restore_map();
        tl = tl->next;
    }
    return total;
}

int two_star()
{
    int total = 0;
    trailhead_list_t *tl = trailhead_list;

    if (tl == NULL)
    {
        perror("No trailheads found");
        exit(1);
    }

    while (tl != NULL)
    {
        int num_trails = number_of_trails_extended(tl->trailhead.x, tl->trailhead.y, 0);
        total += num_trails;
        tl = tl->next;
    }
    return total;
}

int main()
{
    parse_input();
    printf("One star: %d\n", one_star());
    printf("Two star: %d\n", two_star());
    return 0;
}

// Function definitions

void parse_input()
{
    int c;
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            c = getchar();
            if (c == EOF || c == '\n')
            {
                perror("Unexpected end of file");
                exit(1);
            }
            if (c == '0')
            {
                trailhead_t *t = calloc(1, sizeof(trailhead_list_t));
                t->x = x;
                t->y = y;
                trailhead_list_t *tl = malloc(sizeof(trailhead_list_t));
                tl->trailhead = *t;
                tl->next = trailhead_list;
                trailhead_list = tl;
            }

            map[y][x] = c - '0';
        }
        getchar(); // Skip newline
    }
}

int number_of_trails(int x, int y, char value)
{
    if ((value < 0) || (value > 9))
    {
        printf("Invalid value: %d\n", value);
        exit(1);
    }

    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
    {
        return 0; // Out of bounds
    }

    if (map[y][x] != value)
    {
        return 0;
    }

    if (value == 9) // To stop duplicate trail endings
    {
        map[y][x] = TEMP;
        return 1;
    }

    return number_of_trails(x + 1, y, value + 1) +
           number_of_trails(x - 1, y, value + 1) +
           number_of_trails(x, y + 1, value + 1) +
           number_of_trails(x, y - 1, value + 1);
}

int number_of_trails_extended(int x, int y, char value)
{
    if ((value < 0) || (value > 9))
    {
        printf("Invalid value: %d\n", value);
        exit(1);
    }

    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
    {
        return 0; // Out of bounds
    }

    if (map[y][x] != value)
    {
        return 0;
    }

    if (value == 9) // Duplicate trail endings are allowed
    {
        return 1;
    }

    return number_of_trails_extended(x + 1, y, value + 1) +
           number_of_trails_extended(x - 1, y, value + 1) +
           number_of_trails_extended(x, y + 1, value + 1) +
           number_of_trails_extended(x, y - 1, value + 1);
}

void restore_map()
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (map[y][x] == TEMP)
            {
                map[y][x] = 9;
            }
        }
    }
}