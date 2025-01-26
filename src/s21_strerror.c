#include "s21_strerror.h"

#include "s21_string.h"

const int s21_nerr = sizeof(s21_errlist) / sizeof(s21_errlist[0]);

#define ERROR_BUFFER_SIZE 512
char error_buffer[ERROR_BUFFER_SIZE];

char *s21_strerror(int errnum) {
  if (errnum >= 0 && errnum < s21_nerr) {
    s21_strncpy(error_buffer, s21_errlist[errnum], ERROR_BUFFER_SIZE);
  } else {
    s21_sprintf(error_buffer, UNKNOWN_ERROR_MSG, errnum);
  }
  return error_buffer;
}
