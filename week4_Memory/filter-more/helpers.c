#include<math.h>
#include "helpers.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    
    for(int i = 0;i < height;i++)
    {
        for(int j = 0;j < width;j++)
        {
            BYTE Gray = round( (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0 );
            image[i][j].rgbtRed = Gray;
            image[i][j].rgbtGreen = Gray;
            image[i][j].rgbtBlue = Gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0;i < height;i++)
    {
        for(int j = 0;j < width / 2;j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    // Copy the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Apply blur
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed = 0, totalGreen = 0, totalBlue = 0;
            int count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        totalRed += copy[ni][nj].rgbtRed;
                        totalGreen += copy[ni][nj].rgbtGreen;
                        totalBlue += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = round((float)totalRed / count);
            image[i][j].rgbtGreen = round((float)totalGreen / count);
            image[i][j].rgbtBlue = round((float)totalBlue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
        for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    int gx[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
        };
    int gy[3][3] = {
        {-1,-2,-1},
        { 0, 0, 0},
        { 1, 2, 1}
        };

    for(int i = 0;i < height;i++)
    {
        for(int j = 0;j < width;j++)
        {
            int gxBlue = 0, gxGreen = 0, gxRed = 0;
            int gyBlue = 0, gyGreen = 0, gyRed = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        int X = gx[di + 1][dj + 1];
                        int Y = gy[di + 1][dj + 1];

                        gxBlue += copy[ni][nj].rgbtBlue * X;
                        gyBlue += copy[ni][nj].rgbtBlue * Y;
                         
                        gxGreen += copy[ni][nj].rgbtGreen * X;
                        gyGreen += copy[ni][nj].rgbtGreen * Y;

                        gxRed += copy[ni][nj].rgbtRed * X;
                        gyRed += copy[ni][nj].rgbtRed * Y;
                    }
                }
            }

            int Red = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int Green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int Blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            if (Red > 255) Red = 255;
            if (Green > 255) Green = 255;
            if (Blue > 255) Blue = 255;

            image[i][j].rgbtRed = Red;
            image[i][j].rgbtGreen = Green;
            image[i][j].rgbtBlue = Blue;

        }
    }
    return;
}
