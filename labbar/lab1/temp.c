#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool is_digit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    } else 
    {
        return false;
    }
}

bool is_number(char *str)
{
    int length = strlen(str);
    
    for(int i = 0; i <= length - 1; i++)
    {
        if (i == 0)
        {
            if(str[i] == '+' || str[i] == '-') 
            {
                if(length == 1) {
                    return false; 
                }
    
                continue;
            }
        }

        if(!is_digit(str[i]))
        {
            return false;
        } 
    }

    return true; 
}

int main(int argc, char *argv[])
{
    if (argc > 1 && is_number(argv[1]))
    {
        printf("%s is a number\n", argv[1]);
    } 
    else 
    {
        if (argc > 1)
        {
            printf("%s is not a number\n", argv[1]);
        }
        else
        {
            printf("Please provide a command line argument!\n");
        }
    }
    return 0; 
}