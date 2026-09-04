#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void clear_keyboard_buffer()
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



int read_string(char *buf, int buf_siz)
{
    int result = 0; 
    do 
    {
        int c = getchar();
        if(c == EOF)
        {
            clear_keyboard_buffer(); 
            return result;
        }
        if(c == '\n')
        {
            return result;
        }
        buf[result] = c;
        result++;
        
    } while(result < buf_siz - 1);
    clear_keyboard_buffer();
    return result; 
}

char *ask_question_string(char *question, char *buf, int buf_siz)
{ 

    do 
    {
        printf("%s\n", question);
        read_string(buf, buf_siz);

    } while (strlen(buf) == 0);

    return strdup(buf); 
}


int ask_question_int (char *question)
{
    int result = 0;
    int conversations = 0; 

    do 
    {
        printf("%s\n", question);
        conversations = scanf("%d", &result);
        
        clear_keyboard_buffer(); 

        putchar('\n'); 

    } while (conversations < 1); 
    
    return result;
}