#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t haystack_length = s21_strlen(haystack);
  s21_size_t needle_length = s21_strlen(needle);
  const char *result = S21_NULL;

  if (haystack_length < needle_length) {
    result = S21_NULL;
  } else if (haystack_length == needle_length &&
             !s21_strncmp(haystack, needle, needle_length)) {
    result = haystack;
  } else if (needle_length == 0) {
    result = haystack;
  } else {
    for (s21_size_t i = 0; i <= haystack_length - needle_length; ++i) {
      for (s21_size_t j = 0; j <= needle_length && !result; ++j) {
        if (haystack[i + j] != needle[j]) {
          result = S21_NULL;
          break;
        } else if (j == needle_length - 1) {
          result = haystack + i;
        }
      }
    }
  }
  return (char *)result;
}
