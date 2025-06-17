#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>


char table[26];

void cypher_char(string key);
int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if(strlen(argv[1])!= 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for(int i = 0;i < strlen(argv[1]); i++)
    {
        bool alpha = isalpha(argv[1][i]);
        if (alpha != true)
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }


    cypher_char(argv[1]);

    string plain_text = get_string("Plain Text : ");
    printf("Cypher Text : ");
    for(int i = 0; i < strlen(plain_text); i++)
    {
        // No encrypion for non alpha charecters
        if(!isalpha(plain_text[i]))
        {
            printf("%c",plain_text[i]);
        }
        else
        {
            // finding the index of charecter in alphabetical order.
            if(isupper(plain_text[i]))
            {
                int index = plain_text[i] - 'A';
                printf("%c", table[index]);
            }
            
            // converting code letter into lower if input is lower.
            else if(islower(plain_text[i]))
            {
                int index = toupper(plain_text[i]) - 'A';
                char low = tolower(table[index]);
                printf("%c",low);
            }
        }
    } 
    printf("\n");
    return 0;

}



// Storing the key in an array as upper case.
void cypher_char(string key)
{
    for(int i = 0; i < 26; i++)
    {
        table[i] = toupper(key[i]);
    }
}