#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int compare = 0;
  for (s21_size_t i = 0; i < n && compare == 0; i++) {
    compare = *((char *)str1 + i) - *((char *)str2 + i);
  }
  return compare;
}
