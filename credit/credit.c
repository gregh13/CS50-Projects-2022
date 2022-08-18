#include <cs50.h>
#include <stdio.h>

int calculate(int i, int digit);

int main(void)
{
    // Asks user for card number, must be between 13 and 16 digits
    long num;
    do
    {
        num = get_long("Number: ");
    }
    while (num < 0 || num > 9999999999999999);

    // Set up variables used below
    long num2 = num;
    int total_sum = 0;
    int length;
    int lead1;
    int lead2;
    // Loops 16 times, the max number a credit card can be
    for (long i = 1; i < 17; i++)
    {
        // this helps for when a card number has less than 16 digits
        if (num2 > 0)
        {
            // special instructions for the final number
            if (0 < num2 && num2 < 10)
            {
                lead1 = num2;
                length = i;
                printf("length = %i\n", length);
                total_sum += calculate(i, num2);
                num2 /= 10;
            }
            else
            {
                // grabs the second to last number, used for checking which company
                if (9 < num2 && num2 < 100)
                {
                    lead2 = num2 % 10;
                }
                int digit = num2 % 10;
                printf("digit: %i\n", digit);
                num2 /= 10;
                printf("remaining: %li\n", num2);
                total_sum += calculate(i, digit);
            }
        }
    }
    // helpful output to check
    printf("last two: %i%i\n", lead1, lead2);
    printf("checksum: %i\n", total_sum);
    if (total_sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        // Specific card company rules/classification
        if (length == 15 && lead1 == 3 && (lead2 == 4 || lead2 == 7))
        {
            printf("AMEX\n");
        }
        else if (length == 16 && lead1 == 5 && lead2 < 6 && lead2 > 0)
        {
            printf("MASTERCARD\n");
        }
        else if ((length == 13 || length == 16) && lead1 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
}


// function that helps to evaluate the numeric value for a given digit according to Luhn's Algorithm
int calculate(int i, int digit)
{
    int num_to_add;
    // alternating place values for digits
    if (i % 2 == 0)
    {
        num_to_add = digit * 2;
        if (digit > 4)
        {
            num_to_add -= 9;
            // this functions the same as "double the digits and add the resulting digits"
        }
    }
    else
    {
        num_to_add = digit;
    }
    printf("num_to_add: %i\n", num_to_add);
    return num_to_add;
}


