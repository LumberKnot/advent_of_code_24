
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines

#define MAX_SIZE (180000) // 20 000 * 9 = 180 000

#define NUM_EMPTY_BLOCKS (10000) // Always less than 10 000 empty blocks (from my input)

// Structs
typedef struct chunk_t // Represents a block of memory
{
    int adress;
    unsigned char size;
} chunk_t;

// Function prototypes

/* Fills memory from standard in*/
void parse_input();

void print_memory();

/** Finds the firs empty block that can fit all values and substitutes them
 *  @param size The size of the block
 *  @param index The index of the first slot in the block
 *  @param value The value that will be written in the block
 *  @return 1 if the block was found, 0 otherwise
 */
int find_empty_block(int size, int index, int value);

// Global variables

int memory_slots = 0; // Number of memory slots
int empty_slots = 0;  // Number of Empty memory slots
int total_slots = 0;  // Total number of memory slots

int total_empty_blocks = 0; // Number of empty blocks

int *memory;
chunk_t empty_blocks[NUM_EMPTY_BLOCKS];

// Main functions

/** Calculates the checksum of the new memory */
long one_star()
{
    long checksum = 0;

    int first;                  // Sweeps the memory from the beginning
    int last = total_slots - 1; // Sweeps the memory from the end

    for (first = 0; first < memory_slots; first++)
    {
        if (memory[first] != -1)
        {
            checksum += memory[first] * first;
            continue;
        }

        // else
        while (memory[last] == -1)
        {
            last--;
        }

        checksum += memory[last] * first;
        last--;
    }
    return checksum;
}

/** Calculates the checksum of the new memory V2 */
long two_star()
{
    long checksum = 0;

    int sweep_index = total_slots - 1; // Sweeps the memory from the end
    int value;                         // The value that will be written in the memory
    int size;                          // Size of the memory block
    while (sweep_index >= 0)
    {
        size = 1;
        value = memory[sweep_index];
        // Remove all empty slots
        while (value == -1)
        {
            sweep_index--;
            value = memory[sweep_index];
        }

        // Count the size of the actual block
        while (sweep_index > 0 && memory[sweep_index - 1] == value)
        {
            size++;
            sweep_index--;
        }

        // We know have the first index, the value and the size of the block
        int success = find_empty_block(size, sweep_index, value);
        sweep_index--;
        // print_memory();
    }
    for (int i = 0; i < total_slots; i++)
    {
        if (memory[i] == -1)
        {
            continue;
        }
        else
        {
            checksum += memory[i] * i;
        }
    }
    return checksum;
}

int main()
{
    parse_input();
    printf("One star: %ld\n", one_star());
    printf("Two star: %ld\n", two_star());
    free(memory);
    return 0;
}

// Function definitions

void parse_input()
{
    memory = malloc(MAX_SIZE * sizeof(int));

    memset(memory, -1, MAX_SIZE * sizeof(int));

    int isMem = 1;

    int file_adress = 0;

    int c, i;

    while ((c = getchar()) != '\n')
    {
        if (c < '0' || c > '9')
        {
            perror("Invalid input");
            exit(EXIT_FAILURE);
        }
        c -= '0';
        if (isMem)
        {
            memory_slots += c;
            for (i = 0; i < c; i++)
            {
                memory[total_slots++] = file_adress;
            }
            file_adress++;
            isMem = 0;
        }
        else
        {
            empty_blocks[total_empty_blocks].adress = total_slots;
            empty_blocks[total_empty_blocks++].size = c;
            empty_slots += c;
            total_slots += c;
            isMem = 1;
        }
    }
    printf("Memory slots: %d\n", memory_slots);
    printf("Empty slots:  %d\n", empty_slots);
    printf("Total slots:  %d\nSanity:       %d\n", total_slots, (memory_slots + empty_slots));
}

void print_memory()
{
    int m;
    for (int i = 0; i < total_slots; i++)
    {
        m = memory[i];
        if (m == -1)
        {
            printf(".");
        }
        else
        {
            printf("%d", m);
        }
    }
    printf(" | ");
    for (int i = 0; i < total_empty_blocks; i++)
    {
        printf("%2d:%-2d ", empty_blocks[i].adress, empty_blocks[i].size);
    }
    printf("\n");
}

int find_empty_block(int size, int index, int value)
{
    for (int i = 0; i < total_empty_blocks; i++)
    {
        chunk_t *block = &empty_blocks[i];
        if (block->adress > index)
        {
            return 0; // Should never increase placement of block
        }
        if (block->adress + block->size > index)
        {
            perror("Empty block overlaps non empty memory");
            exit(EXIT_FAILURE);
        }
        if (block->size >= size)
        {
            for (int j = 0; j < size; j++)
            {
                memory[block->adress + j] = value;
                memory[index + j] = -1; // Remove the old file from the memory
            }
            block->size -= size;
            block->adress += size;
            return 1;
        }
    }
    return 0; // No block was found
}
