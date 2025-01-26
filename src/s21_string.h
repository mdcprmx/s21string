#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef long unsigned s21_size_t;
#define S21_NULL (void *)0

// part 1
void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

// part 2, 3
typedef struct options {
  int minus;
  int plus;
  int space;
  int sharp;
  int zero;
  int error;
  int width;
  int precision;
  char len;
  char spec;
} FORMAT;

#define is_flag(ch) \
  ((ch) == '-' || (ch) == '+' || (ch) == ' ' || (ch) == '#' || (ch) == '0')

#define is_len(ch) ((ch) == 'h' || (ch) == 'l' || (ch) == 'L')

#define is_digit(ch) ((ch) >= '0' && (ch) <= '9')

#define is_spec(ch)                                                           \
  ((ch) == 'c' || (ch) == 'd' || (ch) == 'i' || (ch) == 'f' || (ch) == 's' || \
   (ch) == 'u' || (ch) == '%' || (ch) == 'g' || (ch) == 'G' || (ch) == 'e' || \
   (ch) == 'E' || (ch) == 'x' || (ch) == 'X' || (ch) == 'o' || (ch) == 'n' || \
   (ch) == 'p')

int s21_sprintf(char *str, const char *format, ...);
void s21_flag_parser(const char **format, va_list *argList, FORMAT *fParams);
void s21_flag_set(FORMAT *fParams, const char *format);
char *s21_main_func(char *str, s21_size_t *str_len, FORMAT *fParams,
                    va_list *argList);

char *s21_f_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_lf_to_str(char *str, long double number, FORMAT *fParams);
char *s21_s_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_c_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_wc_to_str(char *str, wchar_t *wstr, s21_size_t len);
char *s21_u_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_d_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_o_to_str(char *str, va_list *argList, FORMAT *fParams);
char *s21_xX_to_str(char *str, va_list *argList, FORMAT *fParams);

char *s21_lf_width_flag(char *str, FORMAT *fParams);
char *s21_ll_width_flag(char *str, FORMAT *fParams);
char *s21_blankplus_flag(char *str, FORMAT *fParams);
char *s21_ll_precis_flag(char *str, FORMAT *fParams);

char *s21_ll_to_str(char *str, long long number);
char *s21_ul_to_str(char *str, unsigned long long number);
char *s21_ol_to_str(char *str, unsigned long long number, int prefix);
char *s21_hl_to_str(char *str, unsigned long long number, FORMAT *fParams);
char *s21_s_to_i(const char *format, int *number, va_list *argList);
char *s21_reverse_str(char *str);
char *s21_ld_to_e(char *str, long double number, FORMAT *fParams);
char *s21_e_exceptions(char *str, long double x, int accuracy);
long double s21_round(long double x, int accuracy);

char *s21_strcat(char *dest, const char *src);
char *s21_strcpy(char *dest, const char *src);

// part 5
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

#endif  // SRC_S21_STRING_H_
