# include <stdio.h>
# include <cs50.h>

int main(void)
{
    // Asks user input, only accepts 1 to 8 as answers
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Loops for rows
    for (int i = 0; i < n; i++)
    {
        // Adds extra spaces to make things line up
        for (int l = 0; l < n - (i + 1); l++)
        {
            printf(" ");
        }
        // First set of hash blocks
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        // Space in middle
        printf("  ");
        // Second set of hask blocks
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        // New line for next row
        printf("\n");
    }
}