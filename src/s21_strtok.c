#include "s21_string.h"

int is_delim(char c, const char *delim) {
  int flag = 0;
  while (*delim != '\0') {
    if (c == *delim) {
      flag = 1;
    }
    delim++;
  }
  return flag;
}

char *s21_strtok(char *str, const char *delim) {
  static char *start_point;
  char *result = S21_NULL;
  if (!str) {
    str = start_point;
  }
  if (str) {
    int all_deligms = 0;
    int regular = 0;
    while (regular == 0) {
      if (is_delim(*str, delim)) {
        str++;
      } else if (*str == '\0') {
        all_deligms = 1;
        regular = 1;
      } else {
        regular = 1;
      }
    }
    if (!all_deligms) {
      result = str;
      int flag = 1;
      while (flag == 1) {
        if (*str == '\0') {
          start_point = str;
          flag = 0;
        }
        if (is_delim(*str, delim)) {
          *str = '\0';
          start_point = str + 1;
          flag = 0;
        }
        str++;
      }
    }
  }
  return result;
}
