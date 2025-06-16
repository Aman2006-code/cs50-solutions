#include<stdio.h>
#include<cs50.h>

int main(void)
{
    // Getting height of the pyramid from user.
    int row;
    while (true)
    {
        row = get_int("How many rows of bricks do you want to place?: ");
        if(row > 0)
        {
            break;
        }
        else 
        {
            printf("Invalid input. Please try again.\n");
        }
    }

    // Left half
    for(int j = row;j > 0;j--)
    {
        for(int i = 1;i < j;i++)
        {
            printf(" ");
        }
        for(int k = row;k >= j;k--)
        {
            printf("#");
        }

        // Center gap
        printf("  ");

        // Right half
        for(int k = row;k >= j;k--)
        {
            printf("#");
        }



        // Going to the next row of bricks
        printf("\n");
    }
        
}