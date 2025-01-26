#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

Suite *get_string_suite();

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = get_string_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

START_TEST(test_strlen) {
  ck_assert_msg(s21_strlen("") == 0,
                "FAILURE! Test for s21_strlen with empty string failed!");
  ck_assert_msg(s21_strlen("abc") == 3,
                "FAILURE! Test for s21_strlen with 'abc' failed!");
  ck_assert_msg(s21_strlen("123 ") == 4,
                "FAILURE! Test for s21_strlen with '123 ' failed!");
  ck_assert_msg(s21_strlen("l\n") == 2,
                "FAILURE! Test for s21_strlen with 'l\\n' failed!");
  ck_assert_msg(s21_strlen("Hello, world!") == 13,
                "FAILURE! Test for s21_strlen with 'Hello, world!' failed!");
}
END_TEST

START_TEST(test_memchr) {
  char test_str[] = "Hello, world!";
  char c = 'o';
  void *result = s21_memchr(test_str, c, s21_strlen(test_str));
  ck_assert_msg(result != S21_NULL,
                "FAILURE! Test for s21_memchr failed to find character 'o'");
  ck_assert_msg(result == (void *)&test_str[4],
                "FAILURE! Test for s21_memchr found the wrong character");
  c = 'z';
  result = s21_memchr(test_str, c, s21_strlen(test_str));
  ck_assert_msg(result == S21_NULL,
                "FAILURE! Test for s21_memchr found a character that should "
                "not be there");
}
END_TEST

START_TEST(test_memcmp) {
  char test_str1[] = "Hello";
  char test_str2[] = "Hello";
  char test_str3[] = "World";
  int result = s21_memcmp(test_str1, test_str2, s21_strlen(test_str1));
  ck_assert_msg(result == 0,
                "FAILURE! Test for s21_memcmp failed to compare equal strings");
  result = s21_memcmp(test_str1, test_str3, s21_strlen(test_str1));
  ck_assert_msg(
      result != 0,
      "FAILURE! Test for s21_memcmp failed to compare different strings");
}
END_TEST

START_TEST(test_insert) {
  const char *src = "Hello, world!";
  const char *str = "C";
  s21_size_t start_index = 7;
  char *result = (char *)s21_insert(src, str, start_index);
  ck_assert_msg(result != S21_NULL,
                "FAILURE! Test for s21_insert failed to allocate memory");
  ck_assert_msg(
      strcmp(result, "Hello, Cworld!") == 0,
      "FAILURE! Test for s21_insert failed to insert string correctly");
  free(result);

  start_index = 0;
  result = (char *)s21_insert(src, str, start_index);
  ck_assert_msg(
      result != S21_NULL,
      "FAILURE! Test for s21_insert failed to allocate memory at start");
  ck_assert_msg(
      strcmp(result, "CHello, world!") == 0,
      "FAILURE! Test for s21_insert failed to insert string at start");
  free(result);

  start_index = s21_strlen(src);
  result = (char *)s21_insert(src, str, start_index);
  ck_assert_msg(
      result != S21_NULL,
      "FAILURE! Test for s21_insert failed to allocate memory at end");
  ck_assert_msg(strcmp(result, "Hello, world!C") == 0,
                "FAILURE! Test for s21_insert failed to insert string at end");
  free(result);

  start_index = s21_strlen(src) + 1;
  result = (char *)s21_insert(src, str, start_index);
  ck_assert_msg(result == S21_NULL,
                "FAILURE! Test for s21_insert did not return NULL for "
                "out-of-bounds index");
}
END_TEST

START_TEST(test_memcpy) {
  char src[100] = "Test string for memcpy";
  char dest_custom[100] = {0};
  char dest_standard[100] = {0};

  s21_memcpy(dest_custom, src, sizeof(src));
  memcpy(dest_standard, src, sizeof(src));

  ck_assert_mem_eq(dest_custom, dest_standard, sizeof(src));
}
END_TEST

START_TEST(test_memset) {
  char str_custom[100];
  char str_standard[100];
  int c = 'A';
  s21_size_t n = sizeof(str_custom);

  s21_memset(str_custom, c, n);
  memset(str_standard, c, n);

  ck_assert_mem_eq(str_custom, str_standard, n);
}
END_TEST

START_TEST(test_strchr) {
  const char *str = "Hello, world!";
  char c = 'o';
  char *result = s21_strchr(str, c);
  ck_assert_msg(result != S21_NULL,
                "FAILURE! Test for s21_strchr failed to find character 'o'");
  ck_assert_msg(result == (void *)&str[4],
                "FAILURE! Test for s21_strchr found the wrong character");

  c = 'z';
  result = s21_strchr(str, c);
  ck_assert_msg(result == S21_NULL,
                "FAILURE! Test for s21_strchr found a character that should "
                "not be there");
}
END_TEST

START_TEST(test_strcspn) {
  const char *str1 = "Hello, world!";
  const char *str2 = " ,!";
  s21_size_t result = s21_strcspn(str1, str2);
  ck_assert_msg(result == 5,
                "FAILURE! Test for s21_strcspn failed to find the first "
                "occurrence of a character from str2");

  const char *str3 = "Hello, world!";
  const char *str4 = "Hello, world!";
  result = s21_strcspn(str3, str4);
  ck_assert_msg(result == 0,
                "FAILURE! Test for s21_strcspn failed to return  0 when no "
                "characters from str2 are in str1");
}
END_TEST

START_TEST(test_strerror) {
  char *error_msg = s21_strerror(0);
  ck_assert_msg(strcmp(error_msg, "Success") == 0,
                "FAILURE! Test for s21_strerror failed to return 'Success' for "
                "error number   0");

  error_msg = s21_strerror(1);
  ck_assert_msg(strcmp(error_msg, "Operation not permitted") == 0,
                "FAILURE! Test for s21_strerror failed to return correct error "
                "message for error number   1");
}
END_TEST

START_TEST(test_strncmp) {
  const char *str1 = "Hello";
  const char *str2 = "Hello, world!";
  int result = s21_strncmp(str1, str2, 5);
  ck_assert_msg(result == 0, "FAILURE! Test for s21_strncmp failed");

  result = s21_strncmp(str1, str2, 6);
  ck_assert_msg(result != 0, "FAILURE! Test for s21_strncmp failed");
}
END_TEST

START_TEST(test_strncpy) {
  char msg_1[50] = "";
  char msg_2[50] = "";
  ck_assert_msg(strcmp(s21_strncpy(msg_1, "", 1), strncpy(msg_2, "", 1)) == 0,
                "FAILURE! Test ");
  ck_assert_msg(
      strcmp(s21_strncpy(msg_1, "TEST?", 6), strncpy(msg_2, "TEST?", 6)) == 0,
      "FAIL");
  ck_assert_msg(
      strcmp(s21_strncpy(msg_1, "HAhaHA", 8), strncpy(msg_2, "HAhaHA", 8)) == 0,
      "FAI");
  ck_assert_msg(strcmp(s21_strncpy(msg_1, "", 2), strncpy(msg_2, "", 2)) == 0,
                "FAILURE! Test ");
}
END_TEST

START_TEST(test_strpbrk) {
  ck_assert_int_eq(
      strcmp(s21_strpbrk("this is", "abs"), strpbrk("this is", "abs")), 0);
  ck_assert_int_eq(
      strcmp(s21_strpbrk("this is", "th"), strpbrk("this is", "th")), 0);
  ck_assert_int_eq(
      strcmp(s21_strpbrk("this is", "hi"), strpbrk("this is", "hi")), 0);
  ck_assert_msg(s21_strpbrk("this is", "") == strpbrk("this is", ""),
                "FAILURE! Test");
  ck_assert_msg(s21_strpbrk("this is", "dplf") == strpbrk("this is", "dplf"),
                "Fail");
}
END_TEST

START_TEST(test_strrchr) {
  const char *str = "Hello, world!";
  char *result = s21_strrchr(str, 'o');
  ck_assert_msg(result != S21_NULL && *result == 'o',
                "FAILURE! Test for s21_strrchr failed");
}
END_TEST

START_TEST(test_strstr_basic) {
  const char *haystack = "Hello, world!";
  const char *needle = "world";
  char *result = s21_strstr(haystack, needle);

  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, "world!");
}
END_TEST

START_TEST(test_strstr_not_found) {
  const char *haystack = "Hello, world!";
  const char *needle = "universe";
  char *result = s21_strstr(haystack, needle);

  ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_strstr_empty_needle) {
  const char *haystack = "Hello, world!";
  const char *needle = "";
  char *result = s21_strstr(haystack, needle);

  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, haystack);
}
END_TEST

START_TEST(test_strstr_needle_equals_haystack) {
  const char *haystack = "Hello, world!";
  const char *needle = "Hello, world!";
  char *result = s21_strstr(haystack, needle);

  ck_assert_ptr_nonnull(result);
  ck_assert_str_eq(result, haystack);
}
END_TEST

START_TEST(test_strtok) {
  char str[] = "Hello, world!";
  const char *delim = " ,!";
  char *token = s21_strtok(str, delim);
  ck_assert_msg(token != S21_NULL && strcmp(token, "Hello") == 0,
                "FAILURE! Test for s21_strtok failed");

  token = s21_strtok(S21_NULL, delim);
  ck_assert_msg(token != S21_NULL && strcmp(token, "world") == 0,
                "FAILURE! Test for s21_strtok failed");
}
END_TEST

START_TEST(test_to_lower) {
  const char *str = "Hello, World!";
  char *lower = (char *)s21_to_lower(str);
  ck_assert_msg(lower != S21_NULL && strcmp(lower, "hello, world!") == 0,
                "FAILURE! Test for s21_to_lower failed");
  free(lower);
}
END_TEST

START_TEST(test_to_upper) {
  const char *str = "Hello, World!";
  char *upper = (char *)s21_to_upper(str);
  ck_assert_msg(upper != S21_NULL && strcmp(upper, "HELLO, WORLD!") == 0,
                "FAILURE! Test for s21_to_upper failed");
  free(upper);
}
END_TEST

START_TEST(test_strncat) {
  char src[] = " World";
  char dest[20] = "Hello";
  char dest_original[20] = "Hello";
  size_t n = 6;

  char *result_custom = s21_strncat(dest, src, n);

  char *result_original = strncat(dest_original, src, n);

  ck_assert_msg(strcmp(result_custom, result_original) == 0,
                "FAILURE! s21_strncat does not match strncat.");
}
END_TEST

START_TEST(test_sprintf_simple_string) {
  char buffer[100];
  int length = s21_sprintf(buffer, "Hello, World!");
  ck_assert_msg(strcmp(buffer, "Hello, World!") == 0,
                "Expected 'Hello, World!', got '%s'", buffer);
  ck_assert_msg(length == 13, "Expected length 13, got %d", length);
}
END_TEST

START_TEST(test_sprintf_integer) {
  char buffer[100];
  int value = 12345;
  int length = s21_sprintf(buffer, "%d", value);
  ck_assert_msg(strcmp(buffer, "12345") == 0, "Expected '12345', got '%s'",
                buffer);
  ck_assert_msg(length == 5, "Expected length 5, got %d", length);
}
END_TEST

START_TEST(test_sprintf_float) {
  char buffer[100];
  double value = 123.456;
  int length = s21_sprintf(buffer, "%.2f", value);
  ck_assert_msg(strcmp(buffer, "123.46") == 0, "Expected '123.46', got '%s'",
                buffer);
  ck_assert_msg(length == 6, "Expected length 6, got %d", length);
}
END_TEST

START_TEST(test_sprintf_string_with_width) {
  char buffer[100];
  char *value = "Hello";
  int length = s21_sprintf(buffer, "%10s", value);
  ck_assert_msg(strcmp(buffer, "     Hello") == 0,
                "Expected '     Hello', got '%s'", buffer);
  ck_assert_msg(length == 10, "Expected length 10, got %d", length);
}
END_TEST

START_TEST(test_ul_to_str) {
  char buffer[100];
  unsigned long long number = 1234567890ULL;

  s21_ul_to_str(buffer, number);
  ck_assert_str_eq(buffer, "1234567890");
}
END_TEST

START_TEST(test_ol_to_str) {
  char buffer[100];
  unsigned long long number = 123ULL;

  s21_ol_to_str(buffer, number, 0);
  ck_assert_str_eq(buffer, "173");

  buffer[0] = '\0';
  s21_ol_to_str(buffer, number, 1);
  ck_assert_str_eq(buffer, "0173");
}
END_TEST

START_TEST(test_hl_to_str) {
  char buffer[100];
  unsigned long long number = 0x1a3F;

  FORMAT fParams = {.spec = 'x'};
  s21_hl_to_str(buffer, number, &fParams);
  ck_assert_str_eq(buffer, "1a3f");

  buffer[0] = '\0';
  fParams.spec = 'X';
  s21_hl_to_str(buffer, number, &fParams);
  ck_assert_str_eq(buffer, "1A3F");
}
END_TEST

START_TEST(test_flag_set) {
  FORMAT fParams = {0};

  s21_flag_set(&fParams, "-");
  ck_assert_int_eq(fParams.minus, 1);

  s21_flag_set(&fParams, "+");
  ck_assert_int_eq(fParams.plus, 1);

  s21_flag_set(&fParams, " ");
  ck_assert_int_eq(fParams.space, 1);

  s21_flag_set(&fParams, "#");
  ck_assert_int_eq(fParams.sharp, 1);

  s21_flag_set(&fParams, "0");
  ck_assert_int_eq(fParams.zero, 1);
}
END_TEST

START_TEST(test_trim) {
  const char *original = "  \t\nTest string for trim  \n\t";
  const char *trim_chars = " \t\n";
  char *trimmed = s21_trim(original, trim_chars);

  ck_assert_str_eq(trimmed, "Test string for trim");

  free(trimmed);
}
END_TEST

START_TEST(test_trim_no_chars) {
  const char *original = "Test string with no trim chars";
  char *trimmed = s21_trim(original, "");

  ck_assert_str_eq(trimmed, original);

  free(trimmed);
}
END_TEST

START_TEST(test_trim_null_chars) {
  const char *original = "Test string with NULL trim chars";
  char *trimmed = s21_trim(original, NULL);

  ck_assert_str_eq(trimmed, original);

  free(trimmed);
}
END_TEST

START_TEST(test_s21_wc_to_str) {
  char result[100] = {0};
  wchar_t input_wstr[] = L"AB";
  s21_size_t len = 2;

  s21_wc_to_str(result, input_wstr, len);

  ck_assert_str_eq(result, "AB");
}
END_TEST

START_TEST(sprintf_1_hex) {
  char str1[100];
  char str2[100];
  char *str3 = "%x Test %X Test %x";
  int val = 0x7a4;
  int val2 = 0x91ba123f;
  int val3 = 0x3123;
  sprintf(str1, str3, val, val2, val3);
  s21_sprintf(str2, str3, val, val2, val3);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_2_hex) {
  char str1[100];
  char str2[100];
  char *str3 = "%lx Test %lX Test %hx GOD %hX";
  long int val = 3088675747373646;
  long val2 = 33030030303;
  unsigned short int val3 = 22600;
  unsigned short val4 = 120;
  sprintf(str1, str3, val, val2, val3, val4);
  s21_sprintf(str2, str3, val, val2, val3, val4);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_3_hex) {
  char str1[200];
  char str2[200];
  char *str3 = "%-6.5x Test %.23x Test %-30.X TEST %.x";
  int val = 3015;
  int val2 = 712;
  int val3 = 99;
  int val4 = 38;
  sprintf(str1, str3, val, val2, val3, val4);
  s21_sprintf(str2, str3, val, val2, val3, val4);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_4_hex) {
  char str1[200];
  char str2[200];
  char *str3 = "%0x Test %0.x Test %0.0X TEST %0X GOD %.x";
  int val = 3015;
  int val2 = 712;
  int val3 = 99;
  int val4 = 2939;
  int val5 = 0123;
  sprintf(str1, str3, val, val2, val3, val4, val5);
  s21_sprintf(str2, str3, val, val2, val3, val4, val5);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_11_hex) {
  char str1[200];
  char str2[200];
  char *str3 = "%0X Test %06X Test %05.7X TEST %0.7X Oof %0.X";
  int val = 32;
  int val2 = 8899;
  int val3 = 91918;
  int val4 = 32311;
  int val5 = 8894;
  sprintf(str1, str3, val, val2, val3, val4, val5);
  s21_sprintf(str2, str3, val, val2, val3, val4, val5);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_12_hex) {
  char str1[200];
  char str2[200];
  char *str3 = "%*X Test %-*X Test %*.*X TEST %.*X";
  int val = 32;
  int val2 = 8899;
  int val3 = 919;
  int val4 = 32311;
  int ast = 2;
  int ast2 = 5;
  int ast3 = 4;
  int ast4 = 10;
  int ast5 = 7;
  sprintf(str1, str3, ast, val, ast2, val2, ast3, ast4, val3, ast5, val4);
  s21_sprintf(str2, str3, ast, val, ast2, val2, ast3, ast4, val3, ast5, val4);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_13_hex) {
  char str1[200];
  char str2[200];
  char *str3 = "%- X Test %- 15X sdasda %- 15X sdsad %- X";
  int val = -3231;
  int val2 = -3231;
  int val3 = 3231;
  int val4 = 3231;
  sprintf(str1, str3, val, val2, val3, val4);
  s21_sprintf(str2, str3, val, val2, val3, val4);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_3_octal) {
  char str1[100];
  char str2[100];
  char *str3 = "%lo Test %lo Test %ho GOD %ho";
  long int val = 3088675747373646;
  unsigned long int val2 = 33030030303;
  unsigned short int val3 = 22600;
  unsigned short val4 = 120;
  sprintf(str1, str3, val, val2, val3, val4);
  s21_sprintf(str2, str3, val, val2, val3, val4);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_5_octal) {
  char str1[200];
  char str2[200];
  char *str3 = "%#o Test %#3o Test %#5.7o TEST %#.7o Oof %#.o";
  int val = 32;
  int val2 = 8899;
  int val3 = 91918;
  int val4 = 32311;
  int val5 = 8894;
  sprintf(str1, str3, val, val2, val3, val4, val5);
  s21_sprintf(str2, str3, val, val2, val3, val4, val5);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(u_negative_number) {
  char s21_buff[100] = "";
  char buff[100] = "";
  int number = -100;
  s21_sprintf(s21_buff, "%u", number);
  sprintf(buff, "%u", number);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(u_int_max_plus_number) {
  char s21_buff[100] = "";
  char buff[100] = "";
  unsigned int number = (unsigned int)INT_MAX + 100;
  s21_sprintf(s21_buff, "%u", number);
  sprintf(buff, "%u", number);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(u_with_zero) {
  char s21_buff[100] = "";
  char buff[100] = "";
  int number = 0;
  s21_sprintf(s21_buff, "%u", number);
  sprintf(buff, "%u", number);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(d_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%5d", 1568);
  sprintf(buff, "%5d", 1568);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(f_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%4f", 15.15);
  sprintf(buff, "%4f", 15.15);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(c_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%10c", 'z');
  sprintf(buff, "%10c", 'z');
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(s_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%3s", "Hello, world!");
  sprintf(buff, "%3s", "Hello, world!");
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(o_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%4o", 256);
  sprintf(buff, "%4o", 256);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(x_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%15x", 15);
  sprintf(buff, "%15x", 15);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

START_TEST(u_width) {
  char s21_buff[100] = "";
  char buff[100] = "";
  s21_sprintf(s21_buff, "%8u", 15678940);
  sprintf(buff, "%8u", 15678940);
  ck_assert_pstr_eq(s21_buff, buff);
}
END_TEST

Suite *get_string_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("S21 StringLib");
  tc_core = tcase_create("Core");

  // part 1
  tcase_add_test(tc_core, test_strlen);
  tcase_add_test(tc_core, test_memchr);
  tcase_add_test(tc_core, test_memcmp);
  tcase_add_test(tc_core, test_insert);
  tcase_add_test(tc_core, test_memcpy);
  tcase_add_test(tc_core, test_memset);
  tcase_add_test(tc_core, test_strchr);
  tcase_add_test(tc_core, test_strcspn);
  tcase_add_test(tc_core, test_strerror);

  tcase_add_test(tc_core, test_sprintf_simple_string);
  tcase_add_test(tc_core, test_sprintf_integer);
  tcase_add_test(tc_core, test_sprintf_float);
  tcase_add_test(tc_core, test_sprintf_string_with_width);
  tcase_add_test(tc_core, test_ul_to_str);
  tcase_add_test(tc_core, test_ol_to_str);
  tcase_add_test(tc_core, test_hl_to_str);
  tcase_add_test(tc_core, test_flag_set);
  tcase_add_test(tc_core, test_s21_wc_to_str);

  tcase_add_test(tc_core, test_strncat);
  tcase_add_test(tc_core, test_strncmp);
  tcase_add_test(tc_core, test_strncpy);
  tcase_add_test(tc_core, test_strpbrk);
  tcase_add_test(tc_core, test_strrchr);
  tcase_add_test(tc_core, test_strstr_basic);
  tcase_add_test(tc_core, test_strstr_not_found);
  tcase_add_test(tc_core, test_strstr_empty_needle);
  tcase_add_test(tc_core, test_strstr_needle_equals_haystack);
  tcase_add_test(tc_core, test_strtok);
  tcase_add_test(tc_core, test_to_lower);
  tcase_add_test(tc_core, test_to_upper);
  tcase_add_test(tc_core, test_trim);
  tcase_add_test(tc_core, test_trim_no_chars);
  tcase_add_test(tc_core, test_trim_null_chars);

  // sprintf
  tcase_add_test(tc_core, sprintf_3_octal);
  tcase_add_test(tc_core, sprintf_5_octal);
  tcase_add_test(tc_core, sprintf_1_hex);
  tcase_add_test(tc_core, sprintf_2_hex);
  tcase_add_test(tc_core, sprintf_3_hex);
  tcase_add_test(tc_core, sprintf_4_hex);
  tcase_add_test(tc_core, sprintf_11_hex);
  tcase_add_test(tc_core, sprintf_12_hex);
  tcase_add_test(tc_core, sprintf_13_hex);
  tcase_add_test(tc_core, d_width);
  tcase_add_test(tc_core, f_width);
  tcase_add_test(tc_core, c_width);
  tcase_add_test(tc_core, s_width);
  tcase_add_test(tc_core, o_width);
  tcase_add_test(tc_core, x_width);
  tcase_add_test(tc_core, u_width);
  tcase_add_test(tc_core, u_negative_number);
  tcase_add_test(tc_core, u_int_max_plus_number);
  tcase_add_test(tc_core, u_with_zero);

  suite_add_tcase(s, tc_core);
  return s;
}
