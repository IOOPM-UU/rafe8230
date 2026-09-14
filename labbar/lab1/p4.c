#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    

    int row = atoi(argv[1]);
    int increment = atoi(argv[2]);
    int counter = 1; 
    int total = 0; 

    for (int i = row; i > 0; i--)
    {
        for(int v = increment * counter; v > 0; v--)
        {
            printf("*"); 
            total++;
        }
        counter++; 
        printf("\n");
    }
    printf("total: %d", total);

    return 0;
}