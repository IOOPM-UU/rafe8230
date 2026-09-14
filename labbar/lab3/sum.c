#include <stdio.h>
#include <stdlib.h>

typedef int int_fold_func(int, int);

int add(int a, int b)
{
    return a + b; 
}

int_fold_func *name = add; 

/// En funktion som tar en array av heltal, arrayens längd och
/// en pekare till en funktion f av typen Int -> Int -> Int
int foldl_int_int(int numbers[], int numbers_siz, int_fold_func *f)
{
  int result = 0;

  // Loopa över arrayen och för varje element e utför result = f(result, e)
  for (int i = 0; i < numbers_siz; ++i)
  {
    result = f(result, numbers[i]);
  }

  return result;
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Add arguments: \n");
    } else
    {    
        int numbers[argc - 1]; 
        for (int i = 1; i < argc; i++)
        {
            numbers[i - 1] = atoi(argv[i]); 
        }
        
        int result = foldl_int_int(numbers, argc, add); 
        printf("the sum is: %d \n", result); 
    }


    return 0;
}