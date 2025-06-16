#include<stdio.h>
#include<cs50.h>
#include<math.h>

int digit_number(long long number);
int luhns_algorithm(long long card);
int main(void)
{

    // American Express -> 15 digit
    // Mastercard       -> 16 digit
    // Visa             -> 13 digit or 16 digit

    // American Express -> 34 or 37
    // Mastercard       -> 51, 52, 53, 54, or 55
    // Visa             -> 4

    // sum of every alternate digit starting from second last digit is multiplied by 2 and,
    // added with the sum of every alternate digit starting from last digit,
    // -> gives a multiple of '10'
 
    long long card_number = get_long_long("Enter Your credit card number: ");

    int check_sum = luhns_algorithm(card_number);
    int check = check_sum % 10;
    if(check != 0)
    {
        printf("Invalid Credit Card Number\n");
        return 0;
    } 
    long long first_two = card_number;
    while(first_two > 100)
    {
        first_two /= 10;
    }

    int digit = digit_number(card_number);

    if(digit == 16)
    {
        if( first_two == 51 ||
            first_two == 52 ||
            first_two == 53 ||
            first_two == 54 ||
            first_two == 55
        )
          {
            printf("Your card is Mastercard\n");
          }
        else if(first_two / 10 == 4)
        {
            printf("Your card is Visa card\n");
        }
        else
        {
            printf("Invalid Credit Card Number\n");
            return 0;
        }
    }

    else if(digit == 15)
    {
        if( first_two == 34 ||
            first_two == 37
        )
        {
            printf("Your card is American Express\n");
        }
        else
        {
            printf("Invalid Credit Card Number\n");
            return 0;
        }
    }

    else if(digit == 13)
    {
        if(first_two / 10 == 4)
        {
            printf("Your card is Visa card\n");
        }
        else
        {
            printf("Invalid Credit Card Number\n");
            return 0;
        }
    }
    else
    {
        printf("Invalid Credit Card Number\n");
        return 0;
    }






}


// Find the number of digits in credit card number.
int digit_number(long long number)
{
    int x = 0;
    while(number != 0)
    {
        number /= 10;
        x++;
    } 
    return x;
}


// Luhn's Algorithm
int luhns_algorithm(long long card)
{
    int eventh_digit_sum = 0;
    int oddth_digit_sum = 0;
    for(long long i = 100; i <= 10*card; i *= 100)
    {
        int x = (card%i)/(i/10);
        if(x >= 5)
        {
            eventh_digit_sum = eventh_digit_sum + (2*x) / 10 + (2*x) % 10;
        }
        else
        {
            eventh_digit_sum = eventh_digit_sum + 2*x;
        }
    }
    for(long long i = 10; i <= 10*card; i *= 100)
    {
        int y = (card%i)/(i/10);
        oddth_digit_sum = oddth_digit_sum + y;
    }


    return eventh_digit_sum + oddth_digit_sum;
}
