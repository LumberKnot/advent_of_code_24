
/** Uses the fact that we know the input is a 140x140 (char) square*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 140

static char input[SIZE][SIZE];

/** Fills input matrix from stdin
 * @note Assumes input is a 140x140 square
 */
static int fill_input();

/** Calculates the number of XMAS encountered
 *
 */
int one_star()
{
    int total = 0;

    char current; // Current character

    for (int y = 0; y < SIZE; y++)
    {

        char *word;

        for (int x = 0; x < SIZE; x++)
        {
            current = input[y][x];
            if (current != 'X' && current != 'S')
            {
                continue;
            }

            int possition = 0;

            int found;

            if (current == 'X')
            {
                word = "XMAS";
            }
            else
            {
                word = "SAMX";
            }

            //  Horizontal
            if (x <= SIZE - 4)
            {
                found = 1;
                for (possition = 1; possition < 4; possition++)
                {
                    if (input[y][x + possition] != word[possition])
                    {
                        found = 0;
                    }
                }
                total += found;
            }

            // Vertical
            if (y <= SIZE - 4)
            {
                found = 1;
                for (possition = 1; possition < 4; possition++)
                {
                    if (input[y + possition][x] != word[possition])
                    {
                        found = 0;
                    }
                }
                total += found;
            }

            // Diagonal
            if (x <= SIZE - 4 && y <= SIZE - 4)
            {
                found = 1;
                for (possition = 1; possition < 4; possition++)
                {
                    if (input[y + possition][x + possition] != word[possition])
                    {
                        found = 0;
                    }
                }
                total += found;
            }

            // Anti-diagonal
            if (x >= 3 && y <= SIZE - 4)
            {
                found = 1;
                for (possition = 1; possition < 4; possition++)
                {
                    if (input[y + possition][x - possition] != word[possition])
                    {
                        found = 0;
                    }
                }
                total += found;
            }
        }
    }
    return total;
}

/** Calculates the number of X MAS encountered
 *
 */
int two_star()
{
    int total = 0;
    for (int y = 1; y < SIZE - 1; y++)
    {
        for (int x = 1; x < SIZE - 1; x++)
        {
            if (input[y][x] == 'A')
            {
                // Check \ diagonal
                if (!((input[y - 1][x - 1] == 'M' && input[y + 1][x + 1] == 'S') ||
                      (input[y - 1][x - 1] == 'S' && input[y + 1][x + 1] == 'M')))
                {
                    continue;
                }

                // Check / diagonal
                if (!((input[y - 1][x + 1] == 'M' && input[y + 1][x - 1] == 'S') ||
                      (input[y - 1][x + 1] == 'S' && input[y + 1][x - 1] == 'M')))
                {
                    continue;
                }
                total++;
            }
        }
    }
    return total;
}

int main()
{
    fill_input();
    printf("One star: %d\n", one_star());
    printf("Two star: %d\n", two_star());
    return 0;
}

int fill_input()
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

            input[i][j] = (char)c;
        }
        getchar(); // Skip newline
    }
}
