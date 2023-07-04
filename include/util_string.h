#include <stdio.h>
#ifndef _UTIL_STRING_H_

#define _UTIL_STRING_H_

#ifndef NULL
#define NULL ((void*)0)
#endif

int cto_lower(int c);
int cto_upper(int c);
unsigned long long int cstr_len(const char * str);
int cstr_ccmp(const char * str1, const char * str2);
int cstr_cmp(const char * str1, const char * str2);
char *cstr_chr(const char *s, int c);
char * cstr_rchr(const char *s, int c);
char * cstr_ltrim(const char *s, char * new_buffer);
char * cstr_rtrim(const char *s, char * new_buffer);
char * cstr_trim(const char * s, char * new_buffer);
unsigned int cis_digit(int c);
unsigned int cstr_count(const char * str, int ch);
unsigned int cstr_to_uint(const char * str);
char * cstr_cpy(char * dst, const char * src);
char * cstr_ncpy(char * dst, const char * src, unsigned long int n);
char * cstr_reverse(const char * str, char * new_buffer);

#endif
