#include <stdio.h>

int main()
{
    int count = 0;

    for (int i = 1; i <= 10; i++)
    {
        for (int v = i; v > 0; v--)
        {
            printf("*");
            
        }
        count += i;
        printf("\n");
    }

    printf("Total: %d", count); 
    return 0; 

}