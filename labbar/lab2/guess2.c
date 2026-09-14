#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void clear_keyboard_buffer(void)
{
    int c;
    do 
    {
        c = getchar(); 
    } while (c != '\n' && c != EOF);    
}

int read_string(char *buf, int buf_siz)
{
    int result = 0; 
    do 
    {
        int c = getchar();
        if(c == EOF)
        {
            clear_keyboard_buffer(); 
            buf[result] = '\0';
            return result;
        }
        if(c == '\n')
        {
            buf[result] = '\0';
            return result;
        }
        buf[result] = c;
        result++;
        
    } while(result < buf_siz - 1);
    
    clear_keyboard_buffer();
    
    buf[result] = '\0';
    return result; 
}



int ask_question_int(char *question)
{

  int result = 0;
  int conversions = 0;

    do
    {
        printf("%s\n", question);
        conversions = scanf("%d", &result);
        
        clear_keyboard_buffer(); 
        
        putchar('\n');
    } while (conversions < 1);

    return result;
}


char *ask_question_string(char *question, char *buf, int buf_siz)
{

  int read;
  do
    {
      printf("%s\n", question);
      read = read_string(buf, buf_siz); 
    }
  while (read == 0);
  return buf;
}


int main(void)
{
    int buf_size = 255; 
    char buf[buf_size]; 
    
    srand(time(NULL));
    // int number = rand() % 1024;

    int number = 100;

    char *name;

    name = ask_question_string("What is your name?   ", buf, buf_size);
    printf("%s, I'm thinking of a number, can you guess what it is?\n", name); 

    bool gameloop = true;
    int guesses = 0;

    while (gameloop)
    {
        int answer = ask_question_int("Enter a number:   ");

        if(guesses >= 15)
        {
            gameloop = false;
            break;
        }

        if (answer == number)
        {
            guesses++;
            printf("\n\nBingo!\n\n");
            printf("it took %s %d guesses to arrive at %d \n", name, guesses, number); 
            gameloop = false;
            break;
            
        }
        if (answer < number) 
        {
            printf("Too small!\n\n");
            guesses++;
            continue;
        }
        else
        {
            printf("Too large!\n\n");
            guesses++;
            continue;
        } 

    } 
    
    if (guesses > 15)
    {
        printf("%s has guessed too many times! I was thinking of %d", name, number); 
    }
}