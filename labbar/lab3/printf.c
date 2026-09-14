#include <stdio.h>

int string_length(char *string)
{
    int *end = string;
    while (*end != '\0') ++end;
    return end - string; 
}

void printfln(char *string)
{
    int i = 0; 
    int strln = string_length(string);
    while(i < strln)
    {
        putchar(string[i]);
        i++;
    }
    putchar('\n');
}

char *trim(char *str)
{
    char *start = str;
    char *end = start + string_length(str) - 1;

    while (isspace(*start)) ++start;
    while (isspace(*end)) --end;

    char *cursor = str; 
    for (; start <= end; start++)
    {
        *cursor = *start;
    }
    *cursor = '\0';
    return str; 
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s number or int\n\n", argv[0]);
    } 
    else 
    {
        for(int i = 1; i < argc; i++)
        {
            printf("printf: %s\n", argv[i]);
            printf("ln: ");
            printfln(argv[i]);

        }
    }
    return 0;
}