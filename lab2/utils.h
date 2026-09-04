#include <stdbool.h>

void clear_keyboard_buffer();

bool is_digit(char c);

bool is_number(char *str);

int read_string(char *buf, int buf_siz);

char *ask_question_string(char *question, char *buf, int buf_siz);

int ask_question_int(char *question);