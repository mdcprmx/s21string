#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *out = S21_NULL;
  do {
    if (*str == c) {
      out = str;
    }
  } while (*str++);
  return (char *)out;
}
