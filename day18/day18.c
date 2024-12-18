
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAP_SIZE 71            // 71x71 map (0-70)
#define MAX_CORRUPTIONS (3500) // Plenty of space for corruptions

// Function prototypes

/** Parses input from stdin */
static void parse_input();

// Structs

typedef struct coord
{
    int x;
    int y;
} coord;

// Global variables

static coord *corruptions;
static int num_corruptiopns = 0;

static const int start_x = 0;
static const int start_y = 0;

static const int end_x = 70;
static const int end_y = 70;

// Main functions

/** Solves shortest path to (70,70) for (0,0) after 1024 corruptions*/
static int one_star()
{
    int map[MAP_SIZE][MAP_SIZE];

    // Sett all values to INT_MAX
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            map[y][x] = INT_MAX;
        }
    }

    // Apply corruptions
    for (int i = 0; i < 1024; i++)
    {
        map[corruptions[i].x][corruptions[i].y] = -1;
    }

    // BFS search
    coord *queue = malloc(MAP_SIZE * MAP_SIZE * sizeof(coord));
    int head = 0;
    int num_elements = 0;

    // Add start to queue
    map[start_x][start_y] = 0; // Start value
    queue[head + num_elements].x = start_x;
    queue[head + num_elements].y = start_y;
    num_elements++;

    // BFS
    while (num_elements > 0)
    {
        int x = queue[head].x;
        int y = queue[head].y;
        num_elements--;
        if (num_elements > 0)
        {
            head++;
        }

        int value = map[y][x];

        // Try four directions
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {-1, 1, 0, 0};

        // This loop is wrong should test only 4 directions but not the diagonals
        for (int operation = 0; operation < 4; operation++)
        {
            int new_x = x + dx[operation];
            int new_y = y + dy[operation];

            // Check outside map
            if (new_x < 0 || new_x >= MAP_SIZE || new_y < 0 || new_y >= MAP_SIZE)
            {
                continue;
            }

            // Check if corrupted
            if (map[new_y][new_x] == -1)
            {
                continue;
            }

            // Check if new value is smaller
            if (value + 1 < map[new_y][new_x])
            {
                map[new_y][new_x] = value + 1;
                queue[head + num_elements].x = new_x;
                queue[head + num_elements].y = new_y;
                num_elements++;
            }
        }
    }
    free(queue);
    return map[end_y][end_x];
}

/** Solves the coordinate that causes the maze to be unsolvable
 * This is done in reverse by removing the corruptions one by one until a path is posible
 */
static coord two_star()
{
    int map[MAP_SIZE][MAP_SIZE];

    for (int corruption_index = num_corruptiopns - 1; corruption_index >= 0; corruption_index--)
    {

        // Sett all values to INT_MAX
        for (int y = 0; y < MAP_SIZE; y++)
        {
            for (int x = 0; x < MAP_SIZE; x++)
            {
                map[y][x] = INT_MAX;
            }
        }

        // Apply corruptions
        for (int i = 0; i <= corruption_index; i++)
        {
            map[corruptions[i].x][corruptions[i].y] = -1;
        }

        // BFS search
        coord *queue = malloc(MAP_SIZE * MAP_SIZE * sizeof(coord) * 2); // Safety margin
        int head = 0;
        int num_elements = 0;

        // Add start to queue
        map[start_x][start_y] = 0; // Start value
        queue[head + num_elements].x = start_x;
        queue[head + num_elements].y = start_y;
        num_elements++;

        // BFS
        while (num_elements > 0)
        {
            int x = queue[head].x;
            int y = queue[head].y;
            num_elements--;
            if (num_elements > 0)
            {
                head++;
            }

            int value = map[y][x];

            // Try four directions
            int dx[4] = {0, 0, -1, 1};
            int dy[4] = {-1, 1, 0, 0};

            // This loop is wrong should test only 4 directions but not the diagonals
            for (int operation = 0; operation < 4; operation++)
            {
                int new_x = x + dx[operation];
                int new_y = y + dy[operation];

                // Check outside map
                if (new_x < 0 || new_x >= MAP_SIZE || new_y < 0 || new_y >= MAP_SIZE)
                {
                    continue;
                }

                // Check if corrupted
                if (map[new_y][new_x] == -1)
                {
                    continue;
                }

                // Check if new value is smaller
                if (value + 1 < map[new_y][new_x])
                {
                    map[new_y][new_x] = value + 1;
                    queue[head + num_elements].x = new_x;
                    queue[head + num_elements].y = new_y;
                    num_elements++;
                }
            }
        }
        free(queue);
        // Check if path is possible
        if (map[end_y][end_x] != INT_MAX)
        {
            printf("Last working: (%d,%d)\n", corruptions[corruption_index].x, corruptions[corruption_index].y);
            return corruptions[corruption_index + 1];
        }
    }
}

int main()
{
    parse_input();
    printf("One star: %d\n", one_star());
    coord c = two_star();
    printf("Two star: (%d,%d)\n", c.x, c.y);
    return 0;
}

// Function definitions

static void parse_input()
{
    corruptions = malloc(MAX_CORRUPTIONS * sizeof(coord));
    if (corruptions == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int x, y;
    while (scanf("%d, %d\n", &x, &y) == 2)
    {
        corruptions[num_corruptiopns].x = x;
        corruptions[num_corruptiopns].y = y;
        num_corruptiopns++;
    }
}