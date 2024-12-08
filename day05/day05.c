
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES (1200)      // Arbitrary number based on the input
#define MAX_PAGES (100)       // Every double digit number
#define MAX_UPDATES (200)     // Arbitrary number based on the input
#define MAX_UPDATE_PAGES (30) // Arbitrary number based on the input

#define NOT_FOUND (-1)

typedef struct update_t update_t; // Defines an update
typedef struct pair_t pair_t;     // Defines a before pair

typedef enum PARSE_STATE
{
    RULES,
    UPDATE
} PARSE_STATE;

/** Holds a before relation
 *  @param X The page that needs to be before Y if Y and X are present
 *  @param Y The page that needs to be after X if Y and X are present
 */
struct pair_t
{
    unsigned char X;
    unsigned char Y;
};

static pair_t *rules; // Stores all before rules
static int number_of_rules = 0;

static update_t *updates;
static int number_of_updates = 0;

/** Formats and prints all rules */
static void print_rules();

/** Formats and prints all updates */
static void print_updates();

struct update_t
{
    unsigned char *pages;
    unsigned char number_of_pages;
    unsigned char is_approved;
};

void parse_input()
{
    // Allocate memory for the rules
    rules = malloc(MAX_RULES * sizeof(pair_t));

    updates = malloc(MAX_UPDATES * sizeof(update_t));

    char *line = NULL;
    size_t len = 0;
    size_t bytes_read;
    int page_index = 0;

    int state = RULES;

    while ((bytes_read = getline(&line, &len, stdin)) != -1)
    {
        if (state == RULES)
        {
            if (bytes_read == 1) // Empty line
            {
                state = UPDATE;
                continue;
            }
            sscanf(line, "%hhu|%hhu", &rules[number_of_rules].X, &rules[number_of_rules].Y);
            number_of_rules++;
        }
        else
        {
            // Allocate memory for the pages
            unsigned char *pages = malloc(MAX_UPDATE_PAGES * sizeof(unsigned char));
            unsigned char number_of_pages = 0;

            // Because we know that each page is exactly 2 digits we can do some shortcuts
            char *pointer = line;
            unsigned char page;
            while (bytes_read > 3)
            {
                sscanf(pointer, "%hhu,", &page);
                pages[number_of_pages++] = page;
                pointer += 3;
                bytes_read -= 3;
            }

            if (bytes_read != 3)
            {
                // Should be dd\n
                perror("Something went wrong");
                exit(1);
            }

            sscanf(pointer, "%hhu\n", &page);
            pages[number_of_pages++] = page;

            updates[number_of_updates].pages = pages;
            updates[number_of_updates].number_of_pages = number_of_pages;

            number_of_updates++;
        }
    }

    free(line);
}

static void free_list()
{
    free(rules);

    for (int i = 0; i < 200; i++)
    {
        free(updates[i].pages);
    }
    free(updates);
}

/** Validates every update and markes it as aproved or not
 *  @note Requires an allready created rules and updates list
 */
static void validate_updates();

/** Validates a single update, or returns index of values that are incorrectly placed
 *  @param update The update to be validated
 *  @return index(Y) << 8 | index(X) if there is a problem, -1 otherwise
 *  @note index is always less than 256
 */
static int problem_index(update_t *update);

/** Calculates number of aproved updates
 *  @note Requires validate_updates to be called first
 *  @return The number of updates that are approved scaled by midle value
 */
static int one_star()
{
    int output = 0;

    for (int i = 0; i < number_of_updates; i++)
    {
        if (updates[i].is_approved)
        {
            output += updates[i].pages[updates[i].number_of_pages / 2];
        }
    }

    return output;
}

/** Calculates same value as onestar for failed updates after they are fixed
 * @note Requires validate_updates to be called first
 * @note Assumes there is exactly one ordering that would work THIS IS NOT TRUE
 */
static int two_star()
{
    int output = 0;
    update_t current_update;

    unsigned char temp_Y; // Temporary variable used for sorting

    int index_x, index_y;

    for (int update_id = 0; update_id < number_of_updates; update_id++)
    {
        if (updates[update_id].is_approved)
        {
            continue;
        }
        current_update = updates[update_id];

        int cur_problem_index = problem_index(&current_update);
        while (cur_problem_index != -1)
        {
            index_x = cur_problem_index & 0xFF;
            index_y = (cur_problem_index & 0xFF00) >> 8;

            temp_Y = current_update.pages[index_y];
            current_update.pages[index_y] = current_update.pages[index_x];
            current_update.pages[index_x] = temp_Y;

            cur_problem_index = problem_index(&current_update);
        }

        // corretly sorted now
        output += current_update.pages[current_update.number_of_pages / 2];
    }
    return output;
}

int main()
{
    parse_input();
    validate_updates();

    printf("One star: %d\n", one_star());
    printf("Two star: %d\n", two_star());

    free_list();
    return 0;
}

void print_rules()
{
    for (int i = 0; i < number_of_rules; i++)
    {
        printf("[%4d] %2d must come before %2d\n", i, rules[i].X, rules[i].Y);
    }
}

void print_updates()
{
    for (int i = 0; i < number_of_updates; i++)
    {
        printf("[%3d] ", i);
        for (int j = 0; j < updates[i].number_of_pages; j++)
        {
            printf("%2d, ", updates[i].pages[j]);
        }
        printf("\n");
    }
}

int problem_index(update_t *update)
{
    int index[MAX_PAGES];
    // Fills with NOT_FOUND
    memset(index, NOT_FOUND, MAX_PAGES * sizeof(int));

    unsigned char *pages = update->pages;
    unsigned char number_of_pages = update->number_of_pages;

    // Fills with the first occurance of the page
    for (int i = 0; i < number_of_pages; i++)
    {
        if (index[pages[i]] == NOT_FOUND)
        { // number_of_updates
            index[pages[i]] = i;
        }
    }

    // Check if the rules are respected
    for (int rule_id = 0; rule_id < number_of_rules; rule_id++)
    {
        if (index[rules[rule_id].Y] != NOT_FOUND)
        {
            if (index[rules[rule_id].X] == NOT_FOUND)
            {
                // X is not present, so we can continue
                continue;
            }
            if (index[rules[rule_id].X] > index[rules[rule_id].Y])
            {
                return ((index[rules[rule_id].Y] << 8) | index[rules[rule_id].X]);
            }
        }
    }
    return -1; // Success
}

void validate_updates()
{

    unsigned char *pages;
    unsigned char number_of_pages;

    for (int update_id = 0; update_id < number_of_updates; update_id++)
    {

        if (problem_index(&updates[update_id]) == -1)
        {
            updates[update_id].is_approved = 1;
        }
        else
        {
            updates[update_id].is_approved = 0;
        }
    }
}
