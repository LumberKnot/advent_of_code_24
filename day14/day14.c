
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROBOTS (500) // Number of robots in my input

#define MAP_WIDTH (101)
#define MAP_HEIGHT (103)

typedef struct Robot
{
    int x;
    int y;
    int v_x;
    int v_y;
} Robot;

// Function prototypes

/** Parses input from std in */
static void parse_input();

/** Takes one step for each robot */
static void step();

// Global variables
Robot robots[NUM_ROBOTS];

// Main functions

static int one_star()
{

    for (int i = 0; i < 100; i++)
    {
        step();
    }
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0; // Quadrants

    for (int i = 0; i < NUM_ROBOTS; i++)
    {
        if ((robots[i].x == (MAP_WIDTH / 2)) || (robots[i].y == (MAP_HEIGHT / 2)))
        {
            // On the middle line do not count
            continue;
        }

        if (robots[i].x < MAP_WIDTH / 2)
        {
            // Left side
            if (robots[i].y < (MAP_HEIGHT / 2))
            {
                // Top
                q1++;
            }
            else
            {
                // Bottom
                q3++;
            }
        }
        else
        {
            // Right side
            if (robots[i].y < (MAP_HEIGHT / 2))
            {
                // Top
                q2++;
            }
            else
            {
                // Bottom
                q4++;
            }
        }
    }
    return (q1 * q2 * q3 * q4);
}

/* Going to look for the chrismast tree manualy*/
static void two_star()
{
    // 6577 steps to make the chrismast tree
    char map[MAP_HEIGHT][MAP_WIDTH];
    memset(map, ' ', sizeof(map));

    for (int i = 0; i < 6577 - 100; i++) // 100 steps are already done in one_star
    {
        step();
    }
    for (int j = 0; j < NUM_ROBOTS; j++)
    {
        map[robots[j].y][robots[j].x] = '#';
    }
    for (int k = 0; k < MAP_WIDTH; k++)
    {
        printf("-");
    }
    printf("\n");
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
    for (int k = 0; k < MAP_WIDTH; k++)
    {
        printf("-");
    }
    printf("\n");
    printf("Step %d\n\n\n", 6577);
}

int main()
{
    parse_input();
    printf("One star: %d\n", one_star());
    two_star();
    return 0;
}

// Function definitions
static void parse_input()
{

    int i;
    for (i = 0; i < NUM_ROBOTS; i++)
    {
        int scaned = scanf("p=%d,%d v=%d,%d\n", &robots[i].x, &robots[i].y, &robots[i].v_x, &robots[i].v_y);
        if (scaned != 4)
        {
            perror("Error parsing input\n");
            exit(1);
        }
    }
}

void step()
{
    for (int i = 0; i < NUM_ROBOTS; i++)
    {
        robots[i].x = (robots[i].x + robots[i].v_x) % MAP_WIDTH;
        robots[i].y = (robots[i].y + robots[i].v_y) % MAP_HEIGHT;

        // Check that no-one is out of bounds
        if (robots[i].x < 0)
        {
            robots[i].x = MAP_WIDTH + robots[i].x;
        }
        if (robots[i].y < 0)
        {
            robots[i].y = MAP_HEIGHT + robots[i].y;
        }
    }
}
