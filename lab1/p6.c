#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]); 

    while (num1 != num2)
    {
        if (num1 > num2)
        {
            num1 -= num2;
        } else
        {
            num2 -= num1;
        }
    }
    printf("%d is the gcd", num1); 

}