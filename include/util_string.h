#include <stdio.h>
#ifndef _UTIL_STRING_H_

#define _UTIL_STRING_H_

#ifndef NULL
#define NULL ((void*)0)
#endif

int to_lower(int c);
int to_upper(int c);
unsigned long long int str_len(const char * str);
int str_ccmp(const char * str1, const char * str2);
int str_cmp(const char * str1, const char * str2);
char *str_chr(const char *s, int c);
char * str_rchr(const char *s, int c);
char * str_ltrim(const char *s, char * new_buffer);
char * str_rtrim(const char *s, char * new_buffer);
char * str_trim(const char * s, char * new_buffer);
unsigned int is_digit(int c);
unsigned int str_count(const char * str, int ch);
unsigned int str_to_uint(const char * str);
char * str_cpy(char * dst, const char * src);
char * str_ncpy(char * dst, const char * src, unsigned long int n);
char * str_reverse(const char * str, char * new_buffer);

#endif
