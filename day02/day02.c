
#include <stdio.h>
#include <stdlib.h>

#define MAX_REPORTS (1000) // From my input file. (preferably not used)
#define MAX_LEVELS (8)     // From my input file. (preferably not used)

/** One row of the input file. */
typedef struct report_t
{
    int size;
    int *levels;
} report_t;

// All reports
static report_t *reports = NULL;

/* Creates array of reports from std in */
void fill_reports();

void free_reports();

/** Checks one report to see if it is both
 *  1) Strictly increasing or strictly decreasing
 *  2) Increasing or decreasing by 1 2 or 3
 */
int one_star_approved(report_t *report);

/** Checks one report to see if it is both but it can ignore one element
 *  1) Strictly increasing or strictly decreasing
 *  2) Increasing or decreasing by 1 2 or 3
 */
int two_star_approved(report_t *report);

/** Counts the number of reports that are one star approved */
int one_star()
{
    int count = 0;
    for (int i = 0; i < MAX_REPORTS; i++)
    {
        count += one_star_approved(&reports[i]);
    }
    return count;
}

int two_star()
{
    int count = 0;
    for (int i = 0; i < MAX_REPORTS; i++)
    {
        count += two_star_approved(&reports[i]);
    }
    return count;
}

int main()
{

    fill_reports();
    printf("One star: %d\n", one_star());
    printf("Two star %d\n", two_star());
    free_reports();
}

int one_star_approved(report_t *report)
{
    int diffs[MAX_LEVELS - 1];
    if (report->size == 0)
    {
        return 0;
    }

    for (int i = 0; i < report->size - 1; i++)
    {
        int diff = report->levels[i + 1] - report->levels[i];
        if (diff == 0 || abs(diff) > 3)
            return 0;

        diffs[i] = report->levels[i + 1] - report->levels[i];
    }

    // Check sign
    int is_positive = diffs[0] > 0;
    if (is_positive)
    {
        for (int i = 1; i < report->size - 1; i++)
        {
            if (diffs[i] < 0)
            {
                return 0;
            }
        }
    }
    else
    {
        for (int i = 1; i < report->size - 1; i++)
        {
            if (diffs[i] > 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

int two_star_approved(report_t *report)
{
    if (one_star_approved(report))
    {
        return 1;
    }

    // Ignoring every element once
    int diffs[MAX_LEVELS - 2];

    // Ignore is the index that is ignored
    for (int ignore = 0; ignore < report->size; ignore++)
    {
        int success = 1;
        int index_first = 0, index_second;
        for (int fill_index = 0; fill_index < (report->size - 2); fill_index++)
        {
            if (index_first == ignore)
            {
                index_first++;
            }

            index_second = index_first + 1;

            if (index_second == ignore)
            {
                index_second++;
            }
            if (index_second == report->size)
            {
                printf("Oh fuck");
                exit(1);
            }

            int diff = report->levels[index_second] - report->levels[index_first];
            if (diff == 0 || abs(diff) > 3)
            {
                success = 0;
                break; // Try new ignore
            }
            diffs[fill_index] = diff;
            index_first++;
        }
        if (!success)
        {
            continue; // falied
        }

        int is_positive = diffs[0] > 0;
        if (is_positive)
        {
            for (int i = 1; i < report->size - 2; i++)
            {
                if (diffs[i] < 0)
                {
                    success = 0;
                    break;
                }
            }
        }
        else
        {
            for (int i = 1; i < report->size - 2; i++)
            {
                if (diffs[i] > 0)
                {
                    success = 0;
                    break;
                }
            }
        }
        if (success)
            return 1;
    }
    return 0;
}

void fill_reports()
{
    if (reports != NULL)
    {
        return;
    }
    reports = calloc(MAX_REPORTS, sizeof(report_t));
    int index = 0;
    int new_num = 0;
    int c;
    while (index < MAX_REPORTS)
    {
        reports[index].levels = malloc(MAX_LEVELS * sizeof(int));
        while ((c = getchar()) != '\n')
        {
            if (c == ' ')
            {
                reports[index].levels[reports[index].size++] = new_num;
                new_num = 0;
            }
            else
            {
                new_num = new_num * 10 + c - '0';
            }
        }
        reports[index].levels[reports[index].size++] = new_num;
        new_num = 0;
        index++;
    }
}

void free_reports()
{
    if (reports == NULL)
    {
        return;
    }
    for (int i = 0; i < MAX_REPORTS; i++)
    {
        free(reports[i].levels);
    }
    free(reports);
}