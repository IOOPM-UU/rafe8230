#include <stdio.h>
#include <stdlib.h>

int fib(int num)
{
  if (num < 2)
  {
    return num;
  } else 
  {
    return fib(num - 1) + fib(num - 2); 
  }
}


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    int n = atoi(argv[1]);
    if (n < 2)
    {
      printf("fib(%d) = %d\n", n, n);
    }
    else
    {
      printf("fib(%s) = %d\n", argv[1], fib(n));
    }
  }
  return 0;
}


