#include "s21_string.h"

char* s21_strncat(char* dest, const char* src, s21_size_t n) {
  char* out = dest;
  dest = dest + s21_strlen(dest);
  while (n > 0 && *src != 0) {
    *dest = *src;
    dest++;
    src++;
    n--;
  }
  *dest = '\0';
  return out;
}

char* s21_strcat(char* dest, const char* src) {
  char* ptr = dest + s21_strlen(dest);

  while (*src != '\0') {
    *ptr++ = *src++;
  }

  *ptr = '\0';

  return dest;
}
