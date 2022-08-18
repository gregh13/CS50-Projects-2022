#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int calculate_level(float letters, float words, float sentences);

int main(void)
{
    string text = get_string("Text: ");
    int letters = 0;
    // Offset for last word not having a space after it
    int words = 1;
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Catches only letter
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))
        {
            letters++;
        }
        // Catches the spaces
        if (text[i] == 32)
        {
            words++;
        }
        // Catches sentence punctuation
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentences++;
        }
    }
    // Code to help verify correctness
    // printf("letters: %i\n", letters);
    // printf("words: %i\n", words);
    // printf("sentences: %i\n", sentences);
    int grade_level = calculate_level(letters, words, sentences);
    if (grade_level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade_level > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_level);
    }
}

int calculate_level(float letters, float words, float sentences)
{
    float L = (letters / words) * 100;
    float S = (sentences / words) * 100;
    return round((0.0588 * L) - (0.296 * S) - 15.8);
}

// Space = 32, Period = 46, Question = 63, Exclaimation = 33