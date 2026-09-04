#include <stdio.h>
#include <stdlib.h>

void print_number(int num)
{
    if (num % 3 == 0 && num % 5 == 0) {
        printf("fizz buzz  ");
    } 
    else if (num % 3 == 0) {
     printf("fizz  ");
    }    
    else if (num % 5 == 0) { 
        printf("buzz  ");
    } else {
        printf("%d  ", num);
        printf("teste");
    }

}

int main(int argc, char *argv[])
{
    int count = atoi(argv[1]);
    for (int i = 1; i <= count; i++)
    {
        print_number(i); 
    }
    
    return 0;
}