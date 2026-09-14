#include <stdio.h>
#include <stdbool.h>
#include "utils.h"


int main()  
{
    int result = ask_question_int("Enter a number: "); 
    printf("\n\n\nYou typed: %d\n\n\n", result);

    char *result2 = ask_question_string("Type a string: ");
    printf("You typed: %s\n\n\n", result2);

    double result3 = ask_question_float("Enter a floating point number: ");
    printf("You typed: %lf\n\n", result3);

    return 0;
}