#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
    if (n == 0)
    {
        return 0;
    } 
    if (n == 1)
    {
        return 1;
    } else 
    {
        return fib(n - 1) + fib(n - 2); 
    }
}

int main(int argc, char *argv[])
{
    int count = atoi(argv[1]);

    for (int i = 0; i < count; i++)
    {
        int n = fib(i);
        printf("%d  ", n); 
    }
    


    return 0; 
}