#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, string argv[])
{
    // Protects against users making mistakes with CL arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    for (int j = 0, m = strlen(argv[1]); j < m; j++)
    {
        if (isalpha(argv[1][j]) == 0)
        {
            printf("Key must contain 26 alphabetical characters.\n");
            return 1;
        }
        int counter = 0;
        for (int k = 0, o = strlen(argv[1]); k < o; k++)
        {
            if (argv[1][k] == argv[1][j])
            {
                counter++;
            }
        }
        if (counter > 1)
        {
            printf("Key must contain 26 unique characters.\n");
            return 1;
        }
    }

    // CL argument is correct
    string p_text = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(p_text); i < n; i++)
    {
        // Capital letters
        if (isupper(p_text[i]))
        {
            char c = toupper(argv[1][p_text[i] - 65]);
            printf("%c", c);
        }
        // Lowercase letters
        else if (islower(p_text[i]))
        {
            char c = tolower(argv[1][p_text[i] - 97]);
            printf("%c", c);
        }
        // Non-letters
        else
        {
            printf("%c", p_text[i]);
        }
    }
    printf("\n");
    return 0;

}