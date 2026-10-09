#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils_i2.h"
#include <stdlib.h>
#include <ctype.h>




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

static answer_t make_string(char *str)
{
    answer_t a;
    a.string_value = strdup(str);
    return a; 
}
static answer_t make_int(char *str)
{
    answer_t a;
    a.int_value = atoi(str);
    return a; 
}

char *ask_question_string(char *question)
{ 
    return ask_question(question, not_empty, make_string).string_value;
}

int ask_question_int (char *question)
{
    answer_t answer = ask_question(question, is_number, make_int);
    return answer.int_value; // svaret som ett heltal
}


static answer_t make_size_u(char *str)
{
    answer_t a;
    a.u = strtoul(str, NULL, 10);
    return a; 
}

bool is_size_u(char *str)
{
    if (*str == '\0')
    {
        return false;
    }
    for (char *p = str; *p; p++)
    {
        if (!isdigit((unsigned char) *p))
        {
            return false;
        }
    }
    return true;
}

int ask_question_size_u(char *question) // NEW FUNCTION FOR DATABASE INLUPP2
{
  answer_t answer = ask_question(question, is_size_u, make_size_u);
  return answer.u; 
}

bool is_shelf(char *input) // Edited to fit inlupp 2 description
{
    return isupper((unsigned char) input[0]) 
        && is_digit((unsigned char) input[1])
        && is_digit((unsigned char) input[2])
        && input[3] == '\0'; 
}

char *ask_question_shelf(char *question)
{
    answer_t answer = ask_question(question, is_shelf, make_string);
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

int read_string_to_buf(char *buf, const int buf_size, const char *string, const char character)
{
    int i = 0;
    while (!(*(string + i) == '\0'))
    {
        if (i >= buf_size)
        {
            break;
        }
    *(buf + i) = *(string + i);
        i++;
    }
    *(buf + i) = character;
    return i;
}

// function cmp_names fabricated by Ai:
static int cmp_names(const void *a, const void *b)
{
    const char *s1 = *(char * const *) a;
    const char *s2 = *(char * const *) b;
    return strcmp(s1, s2);
}

void sort_names_alphabetically(char *names[], size_t size)
{
    qsort(names, size, sizeof(char *), cmp_names); 
}