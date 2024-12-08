
/** Uses the fact that we know the input is a 130x130 (char) square*/

#include <stdio.h>
#include <stdlib.h>

/** Directions the guard can look
 *  (dir+1)%4 is the direction 90 degrees to the right
 */
typedef enum
{
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction;

typedef struct node_t node_t;

struct node_t
{
    int x;
    int y;
    node_t *next;
};

#define SIZE 130

/*Map that also counts if a square has been visited*/
static int main_map[SIZE][SIZE];

static int guard_x_start, guard_y_start;
static direction guard_dir_start = UP; // Guard starts looking up

/* Position of the guard, */
static int guard_x, guard_y;
static direction guard_dir; // Guard starts looking up

/** Fills input matrix from stdin
 * @note Assumes input is a 130x130 square
 * @note Will fill empty cells with 0 and obstacles with -1
 */
static int fill_map();

/** Calculates the number of steps before guard leaves the the mao
 * @note Assumes main_map is filled
 * @return Number of visited squares -1 if guard never leaves the map
 */
static int calculate_steps();

/** Calculates number of visited squares
 * @note Assumes main_map is filled
 */
static int one_star()
{
    guard_x = guard_x_start;
    guard_y = guard_y_start;
    guard_dir = guard_dir_start;
    return calculate_steps(&main_map);
}

/** Calculates the number of positions that would lead to a infinity loop
 * @note Assumes main_map is filled
 * @note Assumes one_star has been called
 */
static int two_star()
{
    /** Rules to try
     * NOT starting position
     *
     * Try all positions that have a non 0 non -1 value
     *  Save position and value
     *  Change it to -1
     *  Calculate steps and add 1 to total if it returns -1
     *  Change it back
     */

    node_t *head = NULL;

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            // Add all positions that are visited in a linked list
            if (x == guard_x_start && y == guard_y_start)
            {
                continue;
            }
            if (main_map[y][x] > 0)
            {
                node_t *new_node = malloc(sizeof(node_t));
                new_node->x = x;
                new_node->y = y;
                new_node->next = head;
                head = new_node;
            }
        }
    }

    int total = 0;

    while (head != NULL)
    {
        int x = head->x;
        int y = head->y;
        node_t *next = head->next;
        main_map[y][x] = -1;

        guard_x = guard_x_start;
        guard_y = guard_y_start;
        guard_dir = guard_dir_start;

        // Reset map
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (main_map[i][j] > 0)
                {
                    main_map[i][j] = 0;
                }
            }
        }

        if (calculate_steps() == -1)
        {
            total++;
        }

        main_map[y][x] = 0;
        free(head);
        head = next;
    }

    return total;
}

int main()
{
    fill_map();

    printf("One star: %d\n", one_star());
    printf("Two star: %d\n", two_star());
    return 0;
}

int fill_map()
{
    int c;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            c = getchar();
            if (c == EOF)
            {
                perror("Error reading input");
                exit(EXIT_FAILURE);
            }
            if (c == '.')
            {
                main_map[i][j] = 0;
            }
            else if (c == '#')
            {
                main_map[i][j] = -1;
            }
            else if (c == '^')
            {
                main_map[i][j] = 0;
                guard_x_start = j;
                guard_y_start = i;
            }
        }
        getchar(); // Skip newline
    }
}

int calculate_steps()
{
    int total = 0;

    // Bitmask that represents the direction the guard is looking
    int direction_mask = 1 << guard_dir;

    int next_x, next_y;

    // While guard is inside the map
    while (guard_x >= 0 && guard_x < SIZE && guard_y >= 0 && guard_y < SIZE)
    {
        // Unvisited square
        if (main_map[guard_y][guard_x] == 0)
        {
            total++;
        }

        // If guard is on a square that has not been visited in the same direcion
        if ((main_map[guard_y][guard_x] & direction_mask) != 0)
        {
            // Direction and position has been visited
            return -1;
        }
        // Else
        main_map[guard_y][guard_x] |= direction_mask;

        do
        {
            // Move guard
            switch (guard_dir)
            {
            case UP:
                next_x = guard_x;
                next_y = guard_y - 1;
                break;
            case DOWN:
                next_x = guard_x;
                next_y = guard_y + 1;
                break;
            case LEFT:
                next_x = guard_x - 1;
                next_y = guard_y;
                break;
            case RIGHT:
                next_x = guard_x + 1;
                next_y = guard_y;
                break;
            default:
                perror("Invalid direction");
                exit(EXIT_FAILURE);
            }
            if (main_map[next_y][next_x] == -1)
            {
                guard_dir = (guard_dir + 1) % 4;
                direction_mask = 1 << guard_dir;
            }

        } while (main_map[next_y][next_x] == -1);

        guard_x = next_x;
        guard_y = next_y;
    }
    return total;
}
