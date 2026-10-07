#include <stdbool.h>


typedef union {
    int int_value;
    float float_value;
    char *string_value;
    char *shelf; 
    char chr;
} answer_t;


typedef bool control_format(char *arr);

typedef answer_t convert(char *arr); 

extern char *strdup(const char *);



void clear_keyboard_buffer(void);

int read_string(char *buf, int buf_siz);

bool is_number(char *str);

bool is_digit(char c);

bool is_float(char *str);

answer_t make_float(char *str);

/// Hjälpfunktion till ask_question_string
bool not_empty(char *str);

answer_t ask_question(char *question, control_format *check, convert *convert);

char *ask_question_string(char *question);

int ask_question_int(char *question);

char *ask_question_shelf(char *question);

double ask_question_float(char *question); 

bool is_valid_char(char *str);

answer_t make_char(char *str);

char ask_question_char(char *question);




