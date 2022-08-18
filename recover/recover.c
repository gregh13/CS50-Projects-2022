#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage ./recover forensic_image\n");
        return 1;
    }
    // Opens file and checks it opened correctly
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Counter for jpg filename, buffer for reading and writing to jpg files
    int counter = 0;
    BYTE buffer[512];
    // Create memory for jpg filename
    char *name_buffer = malloc(8);
    sprintf(name_buffer, "00%i.jpg", counter);
    // Initialize jpg variable so while loop below functions without error
    FILE *jpg = fopen("xxx.jpg", "w");
    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            if (counter == 0)
            {
                // Close initial jpg file, create first real one
                fclose(jpg);
                jpg = fopen(name_buffer, "w");
                fwrite(buffer, 1, BLOCK_SIZE, jpg);
                counter++;
            }
            else
            {
                // Closes previous jpg files, changes filename, opens new jpg file
                fclose(jpg);
                if (counter < 10)
                {
                    sprintf(name_buffer, "00%i.jpg", counter);
                }
                else if (counter < 100)
                {
                    sprintf(name_buffer, "0%i.jpg", counter);
                }
                else if (counter < 1000)
                {
                    sprintf(name_buffer, "%i.jpg", counter);
                }
                counter++;
                jpg = fopen(name_buffer, "w");
                fwrite(buffer, 1, BLOCK_SIZE, jpg);
            }

        }
        else
        {
            // continues to write jpg info when spanning more than one block
            fwrite(buffer, 1, BLOCK_SIZE, jpg);
        }
    }
    // Need to close last jpg file, forensics file, and free filename buffer memory
    fclose(jpg);
    free(name_buffer);
    fclose(file);
}