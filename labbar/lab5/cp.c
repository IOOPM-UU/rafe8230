#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "db.h"



void cat(char *filename)
{
    FILE *f = fopen(filename, "r");
    int c = fgetc(f);
    int i = 1; 

    printf("\n\n\n==== %s ====\n", filename);
    printf("%d. ", i);
    while (c != EOF)
    {
        fputc(c, stdout);
        if (c == '\n') 
        {
          i++;
          printf("%d. ", i);
        }
        c = fgetc(f);
    }

    fclose(f);
    printf("\n");
}

int main(int argc, char *argv[])
{
  // if (argc < 2)
  // {
  //   fprintf(stdout, "Usage: %s fil1 ...\n", argv[0]);
  // }
  // else
  // {
  //   for (int i = 1; i < argc; ++i)
  //   {
  //     cat(argv[i]);
  //   }
  // }

  return 0;
}