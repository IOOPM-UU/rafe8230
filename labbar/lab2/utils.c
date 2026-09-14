#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include <stdlib.h>




/// Hjälpfunktion till ask_question_string
bool not_empty(char *str)
{
  if (strlen(str) > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void clear_keyboard_buffer(void)
{
    int c;
    do 
    {
        c = getchar(); 
    } while (c != '\n' && c != EOF);    
}


bool is_digit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    } else 
    {
        return false;
    }
}

bool is_number(char *str)
{
    int length = strlen(str);
    
    if (length < 1)
    {
        return false;
    }
    

    for(int i = 0; i <= length - 1; i++)
    {
        if (i == 0)
        {
            if(str[i] == '+' || str[i] == '-') 
            {
                if(length == 1) {
                    return false; 
                }
    
                continue;
            }
        }

        if(!is_digit(str[i]))
        {
            return false;
        } 
    }

    return true; 
}

/*
* Arg1 : char *buf : an empty buffer string / array
* Arg 2 : int buf_siz : the size of our buffer
* Returns : the length of a given string (char *buf)
*/

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

answer_t ask_question(char *question, control_format *check_fun, convert *convert_fun)
{
    int buf_size = 255;
    char buf[buf_size]; 

    do 
    {
        printf("%s", question);
        read_string(buf, buf_size);

    } while (!check_fun(buf));

    return convert_fun(buf);
}

char *ask_question_string(char *question)
{ 
    return ask_question(question, not_empty, (convert *) strdup).string_value;
}

int ask_question_int (char *question)
{
  answer_t answer = ask_question(question, is_number, (convert *) atoi);
  return answer.int_value; // svaret som ett heltal
}

bool is_shelf(char *input)
{
    if (*input >= 65 && *input <= 90)
    {
        if (is_number(input + 1))
        {
            return true;
        }
    }
    return false;
}

char *ask_question_shelf(char *question)
{
    answer_t answer = ask_question(question, is_shelf, (convert *) strdup);
    return answer.shelf;

}

answer_t make_float(char *str)
{
  answer_t a;                // skapa ett oinitierat answer_t-värde
  a.float_value = atof(str); // gör det till en float, via atof
  return a;                  // returnera värdet
}

bool is_float(char *str)
{
    int length = strlen(str); 
    int dots = 0;
    int digits = 0;

    if (length == 0)
    {
        return false;
    }

    int i = 0;
    while (i < length)
    {
        
        if (str[i] == '.')
        {
            dots++;
            if (dots > 1) return false;
        } 
        else if (is_digit(str[i]))
        {
            digits++;
        } 
        else
        {
            return false; 
        }
        i++; 
    }
    
    return digits > 0;
}

double ask_question_float(char *question)
{
  return ask_question(question, is_float, make_float).float_value;
}

bool is_valid_char(char *str)
{
    
    // printf("DEBUG: '%s' (length %zu)\n", str, strlen(str));
    if (strlen(str) != 1)
    {
        return false;
    }
    
    char *valid = "LlTtRrGgHhAa";
    while (*valid != '\0')
    {
        if (str[0] == *valid)
        {
            return true;
        } 
        valid++;
    }
    return false;
}

answer_t make_char(char *str)
{
    return (answer_t) { .chr = str[0] };
}


char ask_question_char(char *question)
{
    answer_t answer = ask_question(question, is_valid_char, make_char);
    return answer.chr;
}