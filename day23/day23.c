
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Program consists of nodes connected with connections
 */

#define MAX_CONNECTIONS 676
#define MAX_CLIQUE_SIZE (32) // arbitrary

#ifndef DEBUG
#define INPUT_SIZE (3380)
#else
#define INPUT_SIZE (32)
#endif

// Structs

/** Stores all connections in a linked list
 * Max is 676 connections (26 * 26)
 */
typedef struct connection_list
{
    __uint16_t connection;
    struct connection_list *next;
} connection_list;

// Function prototypes

/** Recursive function to find size of the largest clique
 *  @param clique array of nodes in the already in the clique
 *  @param clique_size current size of the clique
 */
static int max_clique(__uint16_t *clique, unsigned int clique_size);

static void parse_input();

/** Calculates connection id from two chars representing it
 * @param connection two chars representing the connection
 */
static __uint16_t get_connection_id(char *connection);

static void get_connection(__uint16_t id, char *connection);

/** Adds a connection to the list in acending order
 * @param from connection id of the node
 * @param to connection id of the connected node
 */
static void push(__uint16_t from, __uint16_t to);

static void free_connections();

static void print_connections();

static int connects(__uint16_t from, __uint16_t to);

// Global variables

connection_list **connections;
char *best_clique;
unsigned int best_clique_size = 0;

// Main functions

/** For all connections starting with 't' find how many loops of exactly three nodes exist
 */
static int one_star()
{

    /** Three for loops
     *    1. all nodes p starting with 't'
     *       2. all nodes q of p if p < q (To avoid duplicates)
     *          3. see if it connects to original node
     */

    const __uint16_t node_min = get_connection_id("aa");
    const __uint16_t node_max = get_connection_id("zz");

    const __uint16_t t_min = get_connection_id("ta");
    const __uint16_t t_max = get_connection_id("tz");

#ifdef DEBUG
    char *txt = malloc(3 * sizeof(char));
#endif

    int all_count = 0, t_count = 0;

    __uint16_t s, p, q;                        //  q is allways bigger than p and p is bigger than s(tart)
    connection_list *p_list, *q_list, *v_list; // v_list looks for s

    for (s = node_min; s <= node_max; s++)
    {
        p_list = connections[s];
        while (p_list != NULL)
        {
            p = p_list->connection;
            p_list = p_list->next;
            if (p <= s) // Not okay
            {
                continue;
            }

            q_list = connections[p];

            while (q_list != NULL)
            {
                q = q_list->connection;
                q_list = q_list->next;

                if (q <= p) // Not okay
                {
                    continue;
                }
                v_list = connections[q];

                while (v_list != NULL)
                {
                    if (v_list->connection == s)
                    {
                        // Loop of three found
                        all_count++;
                        if ((s >= t_min && s <= t_max) || (p >= t_min && p <= t_max) || (q >= t_min && q <= t_max))
                        {
                            t_count++;
                        }
#ifdef DEBUG
                        get_connection(s, txt);
                        printf("%s,", txt);
                        get_connection(p, txt);
                        printf("%s,", txt);
                        get_connection(q, txt);
                        printf("%s\n", txt);
#endif

                        break;
                    }
                    v_list = v_list->next;
                }
            }
        }
    }
#ifdef DEBUG
    free(txt);
#endif
    return t_count;
}

/** Find the longest group of interconnected nodes
 *  Where each node is connected to all other in the group
 * @return password of comma separated nodes in alphabetical order of the largest group
 */
static void two_star()
{
    /** Will try to do this with a helper method which recursively iterates trying to find
     *  a clique of nodes by applying the ordering rule, never searching for nodes below it's own index
     *  time complexity is O(n!) but is likely to be much faster in practice
     *
     *  Connections will be sorted for speedup
     */

    int best = 0;

    best_clique = malloc(MAX_CLIQUE_SIZE * 3 * sizeof(char));

    __uint16_t *clique = malloc(MAX_CLIQUE_SIZE * sizeof(__uint16_t));

    for (int node_index = 0; node_index < MAX_CONNECTIONS; node_index++)
    {
        if (connections[node_index] == NULL)
        {
            continue;
        }
        clique[0] = node_index;
        int new_clique = max_clique(clique, 1);
        if (new_clique > best)
        {
            best = new_clique;
        }
    }
    free(clique);
    free(best_clique);
    printf("Best: %d\n", best);
}

int main()
{
    parse_input();
    // print_connections();

    // printf("One star: %d\n", one_star());
    two_star();
    free_connections();
    return 0;
}

// Function definitions

int max_clique(__uint16_t *clique, unsigned int clique_size)
{
    int best = clique_size;

    int clique_index;

    connection_list *current = connections[clique[clique_size - 1]];
    __uint16_t max_node = clique[clique_size - 1];
    __uint16_t current_node;

    // Loop through all invalid nodes
    while (current != NULL && current->connection <= max_node)
    {
        // Too small ignore
        current = current->next;
    }

    // For all furhter nodes try to add them to the clique
    while (current != NULL)
    {

        current_node = current->connection;
        current = current->next;

        // Check validity
        for (clique_index = 0; clique_index < clique_size; clique_index++)
        {
            if (!connects(clique[clique_index], current_node))
            {
                // Not valid
                break;
            }
        }
        if (clique_index == clique_size)
        {
            // Valid
            clique[clique_size] = current_node;
            int new_clique = max_clique(clique, clique_size + 1);
            if (new_clique > best)
            {
                best = new_clique;
            }
        }
    }

    if (best > best_clique_size)
    {
        best_clique_size = best;
        for (int i = 0; i < best; i++)
        {
            best_clique[(3 * i)] = clique[i] / 26 + 'a';
            best_clique[(3 * i) + 1] = clique[i] % 26 + 'a';
            best_clique[(3 * i) + 2] = ',';
        }
        best_clique[(3 * best) - 1] = '\0';
        printf("New best: %d %s\n", best, best_clique);
    }

    return best;
}

static void parse_input()
{

    connections = calloc(MAX_CONNECTIONS, sizeof(connection_list));

    char line[6]; // cc-cc\0
    __uint16_t first, second;
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        scanf("%s", line);
        first = get_connection_id(line);
        second = get_connection_id(line + 3);

        // Add connections to respective lists
        push(first, second);
        push(second, first);
    }
}

static __uint16_t get_connection_id(char *connection)
{
    return (connection[0] - 'a') * 26 + (connection[1] - 'a');
}

static void get_connection(__uint16_t id, char *connection)
{
    connection[0] = id / 26 + 'a';
    connection[1] = id % 26 + 'a';
    connection[2] = '\0';
}

static void push(__uint16_t from, __uint16_t to)
{
    connection_list *before = connections[from]; // Node that is before the new connection
    connection_list *new_connection = malloc(sizeof(connection_list));
    new_connection->connection = to;

    if (before == NULL || before->connection > to)
    {
        // First connection

        // NULL if before is NULL before is a missnomer but it's conceptually correct for later
        new_connection->next = before;
        connections[from] = new_connection;

        return;
    }
    while (before->next != NULL && before->next->connection < to)
    {
        before = before->next;
    }

    // Connect is garanteed to be not NULL and lower than to
    new_connection->next = before->next;
    before->next = new_connection;
}

static void free_connections()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        connection_list *current = connections[i];
        while (current != NULL)
        {
            connection_list *next = current->next;
            free(current);
            current = next;
        }
    }
    free(connections);
}

void print_connections()
{
    char *str = malloc(3);
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (connections[i] == NULL)
        {
            continue;
        }
        get_connection(i, str);
        printf("%s: ", str);
        connection_list *current = connections[i];
        while (current != NULL)
        {
            get_connection(current->connection, str);
            printf("%s, ", str);
            current = current->next;
        }
        printf("\n");
    }
    free(str);
}

int connects(__uint16_t from, __uint16_t to)
{
    connection_list *current = connections[from];
    while (current != NULL)
    {
        if (current->connection == to)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
