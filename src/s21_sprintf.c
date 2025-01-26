#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list p_args;
  va_start(p_args, format);
  s21_size_t i = 0, error = 0;
  str = s21_strcpy(str, "");
  while (*format && error == 0) {
    FORMAT fParams = {0};
    fParams.width = fParams.precision = -1;
    fParams.error = fParams.sharp = fParams.zero = fParams.minus =
        fParams.space = fParams.plus = 0;
    if (*format == '%') {
      format++;
      s21_flag_parser(&format, &p_args, &fParams);
      str = s21_main_func(str, &i, &fParams, &p_args);
      error = fParams.error;
    } else {
      str[i++] = *format++;
      str[i] = '\0';
    }
  }
  va_end(p_args);
  return error == 0 ? (int)s21_strlen(str) : -1;
}

void s21_flag_parser(const char **format, va_list *p_args, FORMAT *fParams) {
  if (is_flag(**format))
    while (is_flag(**format)) s21_flag_set(fParams, (*format)++);
  if (is_digit(**format) || **format == '*')
    *format = s21_s_to_i(*format, &fParams->width, p_args);
  if (**format == '.')
    *format = s21_s_to_i(++(*format), &fParams->precision, p_args);
  if (is_len(**format)) fParams->len = *((*format)++);
  if (is_spec(**format)) fParams->spec = *((*format)++);
}

void s21_flag_set(FORMAT *fParams, const char *format) {
  switch (*format) {
    case '-':
      fParams->minus = 1;
      break;
    case '+':
      fParams->plus = 1;
      break;
    case ' ':
      fParams->space = 1;
      break;
    case '#':
      fParams->sharp = 1;
      break;
    case '0':
      fParams->zero = 1;
      break;
  }
}

char *s21_main_func(char *str, s21_size_t *str_len, FORMAT *fParams,
                    va_list *p_args) {
  char *temp_str = calloc(1000 + fParams->width + 2, sizeof(char));
  if (temp_str) {
    switch (fParams->spec) {
      case 'd':
      case 'i':
        s21_d_to_str(temp_str, p_args, fParams);
        break;
      case 'o':
        s21_o_to_str(temp_str, p_args, fParams);
        break;
      case 'u':
        s21_u_to_str(temp_str, p_args, fParams);
        break;
      case 'x':
      case 'X':
        s21_xX_to_str(temp_str, p_args, fParams);
        break;
      case 'f':
        s21_f_to_str(temp_str, p_args, fParams);
        break;
      case 'c':
        s21_c_to_str(temp_str, p_args, fParams);
        break;
      case 's':
        s21_s_to_str(temp_str, p_args, fParams);
        break;
    }
    s21_strcat(str, temp_str);
    free(temp_str);
    *str_len = s21_strlen(str);
  }
  return str;
}

char *s21_f_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  long double ld_number = 0.0;
  double d_number = 0.0;
  double f_number = 0.0;

  switch (fParams->len) {
    case 'l':
      d_number = (double)va_arg(*p_args, double);
      s21_lf_to_str(str, d_number, fParams);
      break;
    case 'L':
      ld_number = va_arg(*p_args, long double);
      s21_lf_to_str(str, ld_number, fParams);
      break;
    default:
      d_number = va_arg(*p_args, double);
      f_number = d_number;
      s21_lf_to_str(str, f_number, fParams);
  }
  s21_blankplus_flag(str, fParams);
  s21_lf_width_flag(str, fParams);
  return str;
}

char *s21_lf_to_str(char *str, long double number, FORMAT *fParams) {
  if (fParams->precision < 0) fParams->precision = 6;
  s21_ld_to_e(str, number, fParams);
  return str;
}

char *s21_s_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  wchar_t *pw = S21_NULL;
  char *p = S21_NULL;

  switch (fParams->len) {
    case 'l':
      pw = va_arg(*p_args, wchar_t *);
      if (!pw) {
        if (fParams->precision < 6)
          s21_strncat(str, "(null)", fParams->precision);
        else
          s21_strcat(str, "(null)");
      } else {
        s21_wc_to_str(str, pw,
                      fParams->precision > -1 ? fParams->precision : 0);
      }
      break;
    default:
      p = va_arg(*p_args, char *);
      if (!p) {
        if (fParams->precision < 6)
          s21_strncat(str, "(null)", fParams->precision);
        else
          s21_strcat(str, "(null)");
      } else {
        if (fParams->precision > -1)
          s21_strncat(str, p, fParams->precision);
        else
          s21_strcat(str, p);
      }
  }
  s21_ll_width_flag(str, fParams);
  return str;
}

char *s21_c_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  wchar_t wchar = 0;
  wchar_t wchar_s[10] = {0};
  switch (fParams->len) {
    case 'l':
      wchar = va_arg(*p_args, wchar_t);
      wchar_s[0] = wchar;
      s21_wc_to_str(str, wchar_s, 0);
      break;
    default:
      str[0] = va_arg(*p_args, int);
      str[1] = '\0';
  }
  s21_ll_width_flag(str, fParams);
  return str;
}

char *s21_wc_to_str(char *str, wchar_t *wstr, s21_size_t len) {
  s21_size_t str_len = s21_strlen(str);
  int cnt = len;
  char *p = str + str_len;

  while (*wstr != '\0' && (cnt-- > 0 || len == 0)) {
    *(p++) = (char)*(wstr++);
  }
  *p = '\0';
  return str;
}

char *s21_u_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  unsigned short us_number;
  unsigned long ul_number;
  unsigned int u_number;

  switch (fParams->len) {
    case 'h':
      us_number = (unsigned short)va_arg(*p_args, unsigned int);
      s21_ll_to_str(str, us_number);
      break;
    case 'l':
      ul_number = (unsigned long)va_arg(*p_args, unsigned long);
      s21_ul_to_str(str, ul_number);
      break;
    default:
      u_number = (unsigned int)va_arg(*p_args, unsigned int);
      s21_ll_to_str(str, u_number);
  }

  s21_ll_precis_flag(str, fParams);
  s21_blankplus_flag(str, fParams);
  s21_ll_width_flag(str, fParams);
  return str;
}

char *s21_d_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  long int ld_number;
  short int sd_number;
  int number;

  switch (fParams->len) {
    case 'h':
      sd_number = (short int)va_arg(*p_args, int);
      s21_ll_to_str(str, sd_number);
      break;
    case 'l':
      ld_number = (long int)va_arg(*p_args, long int);
      s21_ll_to_str(str, ld_number);
      break;
    default:
      number = va_arg(*p_args, int);
      s21_ll_to_str(str, number);
  }
  s21_ll_precis_flag(str, fParams);
  s21_blankplus_flag(str, fParams);
  s21_ll_width_flag(str, fParams);
  return str;
}

char *s21_o_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  unsigned long int ld_number;
  unsigned short int sd_number;
  unsigned int number;

  switch (fParams->len) {
    case 'h':
      sd_number = (unsigned short int)va_arg(*p_args, unsigned int);
      s21_ol_to_str(str, sd_number, fParams->sharp);
      break;
    case 'l':
      ld_number = (unsigned long int)va_arg(*p_args, unsigned long int);
      s21_ol_to_str(str, ld_number, fParams->sharp);
      break;
    default:
      number = va_arg(*p_args, unsigned int);
      s21_ol_to_str(str, number, fParams->sharp);
  }
  s21_ll_precis_flag(str, fParams);
  s21_blankplus_flag(str, fParams);
  s21_ll_width_flag(str, fParams);
  return str;
}

char *s21_xX_to_str(char *str, va_list *p_args, FORMAT *fParams) {
  unsigned long int ld_number;
  unsigned short int sd_number;
  unsigned int number;

  switch (fParams->len) {
    case 'h':
      sd_number = (short unsigned int)va_arg(*p_args, unsigned int);
      s21_hl_to_str(str, sd_number, fParams);
      break;
    case 'l':
      ld_number = (long unsigned int)va_arg(*p_args, unsigned long int);
      s21_hl_to_str(str, ld_number, fParams);
      break;
    default:
      number = (unsigned int)va_arg(*p_args, unsigned int);
      s21_hl_to_str(str, number, fParams);
  }
  s21_ll_precis_flag(str, fParams);
  s21_blankplus_flag(str, fParams);
  s21_ll_width_flag(str, fParams);

  return str;
}

char *s21_lf_width_flag(char *str, FORMAT *fParams) {
  int str_len = (int)s21_strlen(str);
  if (fParams->width > str_len) {
    int padding_size = fParams->width - str_len;
    char padding_char = fParams->zero ? '0' : ' ';
    char *new_str = calloc(fParams->width + 1, sizeof(char));
    if (new_str) {
      if (fParams->minus) {
        s21_memcpy(new_str, str, str_len);
        s21_memset(new_str + str_len, padding_char, padding_size);
      } else {
        s21_memset(new_str, padding_char, padding_size);
        s21_memcpy(new_str + padding_size, str, str_len);
      }
      new_str[fParams->width] = '\0';
      s21_strcpy(str, new_str);
      free(new_str);
    }
  }
  return str;
}

char *s21_ll_width_flag(char *str, FORMAT *fParams) {
  char *ptr;
  ptr = str;
  int str_len = (int)s21_strlen(ptr);
  if (fParams->width > str_len) {
    int padding_size = fParams->width - str_len;
    char padding_char = ' ';
    char *new_str = calloc(fParams->width + 1, sizeof(char));
    if (new_str) {
      if (fParams->minus) {
        s21_memcpy(new_str, ptr, str_len);
        s21_memset(new_str + str_len, padding_char, padding_size);
      } else {
        s21_memset(new_str, padding_char, padding_size);
        s21_memcpy(new_str + padding_size, ptr, str_len);
      }
      new_str[fParams->width] = '\0';
      s21_strcpy(ptr, new_str);
      free(new_str);
    }
  }
  return str;
}

char *s21_blankplus_flag(char *str, FORMAT *fParams) {
  if (is_digit(*str) || *str == 'i') {
    char num_str[2] = "";
    char *p = S21_NULL;
    p = num_str;
    if (fParams->plus)
      *(p++) = '+';
    else if (fParams->space)
      *(p++) = ' ';
    *p = '\0';
    s21_reverse_str(str);
    s21_strcat(str, num_str);
    s21_reverse_str(str);
  }
  return str;
}

char *s21_ll_precis_flag(char *str, FORMAT *fParams) {
  char *num_str = calloc(1000 + fParams->width + 2, sizeof(char));
  if (num_str) {
    s21_size_t str_len = s21_strlen(str);
    int shift;
    char *p = S21_NULL;
    char *s = S21_NULL;
    p = num_str;
    s = str;

    int sign = *s == '-' ? -1 : 0;
    if (fParams->zero == 1)
      fParams->precision = (fParams->precision > fParams->width)
                               ? fParams->precision
                               : fParams->width;

    fParams->precision = fParams->precision > (int)str_len + sign
                             ? fParams->precision
                             : (int)str_len + sign;
    shift = fParams->spec == 'x' && fParams->sharp == 1 ? 32 : 0;

    s21_reverse_str(str);
    for (int i = 0; i < fParams->precision; i++) {
      if (i < (int)str_len + sign)
        *(p++) = *(s++);
      else
        *(p++) = '0';
    }

    if (fParams->sharp == 1 && fParams->spec == (char)('X' + shift)) {
      *(p++) = (char)('X' + shift);
      *(p++) = '0';
    }

    if (sign < 0) *(p++) = '-';
    *p = '\0';
    s21_reverse_str(num_str);
    s21_strcpy(str, num_str);
    free(num_str);
  }
  return str;
}

char *s21_ll_to_str(char *str, long long number) {
  char *p = S21_NULL;
  int flag = 1, sign;

  sign = number < 0 ? -1 : 1;
  number *= sign;
  p = str;

  while (flag) {
    int digit;
    if (number >= 10) {
      digit = (int)(number % 10);
      number = (number - digit) / 10;
    } else {
      digit = (int)number;
      flag = 0;
    }
    *(p++) = (char)(digit + '0');
  }

  if (sign < 0) {
    *(p++) = '-';
  }
  *p = '\0';
  s21_reverse_str(str);
  return str;
}

char *s21_ul_to_str(char *str, unsigned long long number) {
  char *p = S21_NULL;
  int flag = 1;

  p = str;

  while (flag) {
    int digit;
    if (number >= 10) {
      digit = (int)(number % 10);
      number = (number - digit) / 10;
    } else {
      digit = (int)number;
      flag = 0;
    }
    *(p++) = (char)(digit + '0');
  }
  *p = '\0';
  s21_reverse_str(str);
  return str;
}

char *s21_ol_to_str(char *str, unsigned long long number, int prefix) {
  char *p = S21_NULL;
  int flag = 1;

  p = str;

  while (flag) {
    unsigned int digit;
    if (number > 7) {
      digit = number % 8;
      number = (number - digit) / 8;
    } else {
      digit = number;
      flag = 0;
    }
    *(p++) = (char)(digit + '0');
  }

  if (prefix == 1) *(p++) = '0';
  *p = '\0';
  s21_reverse_str(str);
  return str;
}

char *s21_hl_to_str(char *str, unsigned long long number, FORMAT *fParams) {
  char *p = S21_NULL;
  int flag = 1, shift;

  p = str;

  shift = fParams->spec == 'x' ? 32 : 0;

  while (flag) {
    unsigned int digit;
    if (number > 15) {
      digit = number % 16;
      number = (number - digit) / 16;
    } else {
      digit = number;
      flag = 0;
    }
    if (digit < 10)
      *(p++) = (char)(digit + '0');
    else
      *(p++) = (char)(digit + 'A' + shift - 10);
  }
  *p = '\0';
  s21_reverse_str(str);
  return str;
}

char *s21_s_to_i(const char *format, int *number, va_list *p_args) {
  *number = 0;
  while (is_digit(*format)) {
    *number = *format - '0' + *number * 10;
    format++;
  }
  if (*format == '*') {
    *number = va_arg(*p_args, int);
    format++;
  }
  return (char *)format;
}

char *s21_reverse_str(char *str) {
  s21_size_t str_len = s21_strlen(str);
  char ch;
  for (s21_size_t i = 0; i < str_len / 2; i++) {
    ch = str[i];
    str[i] = str[str_len - 1 - i];
    str[str_len - 1 - i] = ch;
  }
  return str;
}

char *s21_ld_to_e(char *str, long double number, FORMAT *fParams) {
  char num1_str[1000] = "", *p1 = S21_NULL;
  p1 = num1_str;
  char num2_str[1000] = "", *p2 = S21_NULL;
  p2 = num2_str;
  long double mantis, right = 0.0L, pow = 1.0L, pow_t = 1.0L;
  int sign;
  sign = signbit(number) == 0 ? 1 : -1;

  if (fpclassify(number) < 4) {
    s21_e_exceptions(num1_str, number, fParams->precision);
    if (sign < 0 && fpclassify(number) != 1) s21_strcat(str, "-");
    s21_strcat(str, num1_str);
    fParams->zero = 0;
  } else {
    number = s21_round(number, fParams->precision);
    mantis = modfl(fabsl(number), &right);

    while (truncl(right / pow) > 0) {
      int digit;
      digit = (int)truncl(fmodl(truncl(right), pow * (long double)10L) / (pow));
      *(p1++) = (char)(digit + '0');
      pow *= 10L;
    }

    if (sign < 0) *(p1++) = '-';
    *p1 = '\0';

    for (int i = 0; i < fParams->precision; i++) pow_t *= 10;

    mantis = roundl(mantis * pow_t);
    pow = 1.0L;
    while (pow < pow_t) {
      int digit;
      digit =
          (int)truncl(fmodl(truncl(mantis), pow * (long double)10L) / (pow));
      *(p2++) = (char)(digit + '0');
      pow *= 10L;
    }
    *p2 = '\0';
    s21_reverse_str(num1_str);
    if (s21_strlen(num1_str) == 0) s21_strcat(str, "0");
    s21_strcat(str, num1_str);
    if (fParams->precision > 0 || fParams->sharp == 1) s21_strcat(str, ".");
    if (fParams->precision > 0) {
      s21_reverse_str(num2_str);
      s21_strcat(str, num2_str);
    }
  }
  return str;
}

char *s21_e_exceptions(char *str, long double x, int precision) {
  switch (fpclassify(x)) {
    case FP_INFINITE:
      s21_strcat(str, "inf");
      break;
    case FP_ZERO:
      s21_strcat(str, "0");
      if (precision > 0) {
        s21_strcat(str, ".");
        while (precision--) s21_strcat(str, "0");
      }
      break;
  }
  return str;
}

long double s21_round(long double x, int precision) {
  long double right = 0.0;
  long double x1 = modfl(fabsl(x), &right);
  int mul = 0;
  int sign = signbit(x) == 0 ? 1 : -1;
  for (int i = 0; i < precision; i++) mul--;

  if (roundl(x1 / powl(10, mul)) > truncl(x1 / powl(10, mul))) {
    x = x + (5 * powl(10, mul - 1)) * sign;
  }
  if (precision <= 6 && x1 > 0) {
    x = x / powl(10, mul);
    x = truncl(x) * powl(10, mul);
  }
  return x;
}