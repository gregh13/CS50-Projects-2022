#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Possible issue adding BYTES together
            float avg_val = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            int grey_val = roundf(avg_val);
            image[i][j].rgbtBlue = grey_val;
            image[i][j].rgbtGreen = grey_val;
            image[i][j].rgbtRed = grey_val;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp_arr)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_arr[i][width - 1 - j] = image[i][j];
        }
        for (int k = 0; k < width; k++)
        {
            image[i][k] = temp_arr[i][k];
        }
    }
    free(temp_arr);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp_array)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int counter = 0;
            float sumb = 0.0;
            float sumg = 0.0;
            float sumr = 0.0;
            // For each pixel, need to run through array again to get averages
            // Need to check if it is within one row or column from the pixel in question
            for (int k = 0; k < height; k++)
            {
                if (k <= (i + 1) && k >= (i - 1))
                {
                    for (int l = 0; l < width; l++)
                    {
                        if (l <= (j + 1) && l >= (j - 1))
                        {
                            sumb += image[k][l].rgbtBlue;
                            sumg += image[k][l].rgbtGreen;
                            sumr += image[k][l].rgbtRed;
                            counter++;
                        }
                    }
                }
            }
            temp_array[i][j].rgbtBlue = round(sumb / counter);
            temp_array[i][j].rgbtGreen = round(sumg / counter);
            temp_array[i][j].rgbtRed = round(sumr / counter);
        }
    }
    // Copy blurred values into output image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_array[i][j];
        }
    }
    free(temp_array);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*tmp_ar)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumbx = 0.0;
            float sumgx = 0.0;
            float sumrx = 0.0;
            float sumby = 0.0;
            float sumgy = 0.0;
            float sumry = 0.0;

            // For each pixel, need to run through array again to get averages
            // Need to check if it is within one row or column from the pixel in question
            for (int k = 0; k < height; k++)
            {
                if (k <= (i + 1) && k >= (i - 1))
                {
                    for (int l = 0; l < width; l++)
                    {
                        // first restrict l to surrounding values
                        if (l <= (j + 1) && l >= (j - 1))
                        {
                            // Gx Weighted Sums (without edge/corner case)

                            // left side, top and bottom
                            if (l == j - 1 && k != i)
                            {
                                // Gx
                                sumbx += -1 * image[k][l].rgbtBlue;
                                sumgx += -1 * image[k][l].rgbtGreen;
                                sumrx += -1 * image[k][l].rgbtRed;
                            }
                            // left side, middle
                            if (l == j - 1 && k == i)
                            {
                                sumbx += -2 * image[k][l].rgbtBlue;
                                sumgx += -2 * image[k][l].rgbtGreen;
                                sumrx += -2 * image[k][l].rgbtRed;
                            }
                            // right side, top and bottom
                            if (l == j + 1 && k != i)
                            {
                                // Gx
                                sumbx += 1 * image[k][l].rgbtBlue;
                                sumgx += 1 * image[k][l].rgbtGreen;
                                sumrx += 1 * image[k][l].rgbtRed;
                            }
                            // right side, middle
                            if (l == j + 1 && k == i)
                            {
                                sumbx += 2 * image[k][l].rgbtBlue;
                                sumgx += 2 * image[k][l].rgbtGreen;
                                sumrx += 2 * image[k][l].rgbtRed;
                            }

                            // Gy Weighted Sums (without edge/corner cases)

                            // top row, left and right
                            if (k == i - 1 && l != j)
                            {
                                sumby += -1 * image[k][l].rgbtBlue;
                                sumgy += -1 * image[k][l].rgbtGreen;
                                sumry += -1 * image[k][l].rgbtRed;
                            }
                            // top row, middle
                            if (k == i - 1 && l == j)
                            {
                                sumby += -2 * image[k][l].rgbtBlue;
                                sumgy += -2 * image[k][l].rgbtGreen;
                                sumry += -2 * image[k][l].rgbtRed;
                            }
                            // bottom row, left and right
                            if (k == i + 1 && l != j)
                            {
                                sumby += 1 * image[k][l].rgbtBlue;
                                sumgy += 1 * image[k][l].rgbtGreen;
                                sumry += 1 * image[k][l].rgbtRed;
                            }
                            // bottom row, middle
                            if (k == i + 1 && l == j)
                            {
                                sumby += 2 * image[k][l].rgbtBlue;
                                sumgy += 2 * image[k][l].rgbtGreen;
                                sumry += 2 * image[k][l].rgbtRed;
                            }
                        }
                    }
                }
            }
            // Add Gx and Gy weight sums, get new val
            int b_val = round(sqrt((sumbx * sumbx) + (sumby * sumby)));
            int g_val = round(sqrt((sumgx * sumgx) + (sumgy * sumgy)));
            int r_val = round(sqrt((sumrx * sumrx) + (sumry * sumry)));

            // Make sure values don't go above 255 (RGB limit)
            if (b_val > 255)
            {
                b_val = 255;
            }
            if (g_val > 255)
            {
                g_val = 255;
            }
            if (r_val > 255)
            {
                r_val = 255;
            }
            tmp_ar[i][j].rgbtBlue = b_val;
            tmp_ar[i][j].rgbtGreen = g_val;
            tmp_ar[i][j].rgbtRed = r_val;
        }
    }
    // Copy blurred values into output image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp_ar[i][j];
        }
    }
    free(tmp_ar);
    return;
}
