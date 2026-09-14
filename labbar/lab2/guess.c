#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <time.h>

int main(void)
{
    srand(time(NULL));
    // int number = rand() % 1024;

    int number = 100;

    char *name;

    name = ask_question_string("What is your name?   ");
    printf("%s, I'm thinking of a number, can you guess what it is?\n", name); 

    bool gameloop = true;
    int guesses = 0;

    while (gameloop)
    {
        int answer = ask_question_int("Enter a number:   ");

        if(guesses > 15)
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