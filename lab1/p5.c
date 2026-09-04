#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int arg = atoi(argv[1]);
    // float temp = sqrt(arg);
    // int limit = floor(temp) + 1;

    for (int a = 2; a < arg - 1; a++)
    {
        for (int b = 2; b < arg - 1; b++)
        {
            int current = a * b;  
                if (arg == current) {
                    printf("%d is not a prime number", arg);
                    return 1; 
                }
                
        }
        
    }
    printf("%d is a prime number", arg); 
    
    return 0;
    
}