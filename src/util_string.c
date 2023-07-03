#include <util_string.h>


char * str_reverse(const char * str, char * new_buffer){
    // reverse the string str and fill the new_buffer
    // and returns a pointer to new_buffer or NULL in error
    // new_buffer must be large enough to keep the reversed string.
    if (!str || !new_buffer)
        return NULL;
    char * tmp = (char*) str;
    while(*tmp)
        ++tmp;
    // tmp points to null
    unsigned int i=0;
    if (str_len(str) == 0){
        new_buffer[0] = '\0';
        return new_buffer;
    }
    tmp--;
    while (tmp >= str){
        new_buffer[i++] = *tmp--;
    }
    new_buffer[i] = '\0';
    return new_buffer;
}

char * str_cpy(char * dst, const char * src){
    // copy src to dst (including null bytes)
    if (!dst || !src)
        return NULL;
    unsigned long int i=0;
    char * tmp = (char*)src;
    for (i=0; *tmp++; ++i){
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return dst;
}

char * str_ncpy(char * dst, const char * src, unsigned long int n){
    unsigned long int i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dst[i] = src[i];
    for ( ; i < n; i++)
        dst[i] = '\0';
    return dst;
}


int to_lower(int c){
    return c >= 'A' && c <= 'Z'? c + 'a' - 'A':c;
}

int to_upper(int c){
    return c >= 'a' && c <= 'z'? c - ('a' - 'A'):c;
}

unsigned long long int str_len(const char * str){
    if (!str)
        return 0;
    char * p = (char*)str;
    unsigned long long int len = 0;
    while (*p++)
        len++;
    return len;
}

// same as strcasecmp. str1 and str2 can not be NULL
int str_ccmp(const char * str1, const char * str2){
    char * s1 = (char *) str1;
    char * s2 = (char *) str2;
    while (*s1 != '\0' && *s2 != '\0' && to_lower(*s1) == to_lower(*s2)){
        s2++;
        s1++;
    }
    return to_lower(*s1) - to_lower(*s2);
}

// str1 and str2 can not be NULL
int str_cmp(const char * str1, const char * str2){
    char * s1 = (char *) str1;
    char * s2 = (char *) str2;
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2){
        s2++;
        s1++;
    }
    return *s1 - *s2;
}

// read man of strchr
char *str_chr(const char *s, int c){
    char * t = (char*) s;
    if (c == '\0')
        return t + str_len(s);
    while (*t != '\0'){
        if (*t == c)
            return t;
        t++;
    }
    return NULL;
}

// read the man page for strrchr
char * str_rchr(const char *s, int c){
    char * t = (char*)s;
    unsigned long long int l = 0;
    l = str_len(s);
    if (c == '\0')
        return t+l;
    t = (char *)s;
    while (l-- > 0)
        if (*(t + l) == c)
            return t + l;
    return NULL;
}

// only trim whitespaces
char * str_ltrim(const char *s, char * new_buffer){
    // return the new string (trimmed) in the new_buffer
    // new_buffer must be large enough to hold the new string
    // it's safe to provide the buffer the same size (+1) as the
    // original string
    if (!new_buffer)
        return NULL;
    if (!s)
        return NULL;
    char * tmp = (char *)s;
    while (*tmp){
        if (*tmp == '\n' || *tmp == ' ' || *tmp == '\f' ||
            *tmp == '\t' || *tmp == '\r' || *tmp == '\v')
            ++tmp;
        else
            break;
    }
    unsigned int i=0;
    while(*tmp)
        new_buffer[i++] = *tmp++;
    new_buffer[i] = '\0';
    return new_buffer;
}

// only trim whitespaces from right side
char * str_rtrim(const char *s, char * new_buffer){
    // returns the new_buffer filled with the rtrimmed
    // string s. The buffer must be large enough to hold
    // the returned result.
    if (!s)
        return NULL;
    if (!new_buffer)
        return NULL;
    unsigned int i=0;
    char * tmp = (char *)s;
    while (*tmp)
        new_buffer[i++] = *tmp++;
    new_buffer[i] = '\0';
    tmp = (char*)new_buffer;
    while(*tmp)
        ++tmp;
    // now tmp points to the \0 character
    if (str_len(s) == 0)    // we don't have anything to trim
        return new_buffer;
    --tmp;
    while(tmp>=s){
        if (*tmp == '\n' || *tmp == ' ' || *tmp == '\f' || *tmp == '\t'
            || *tmp == '\r' || *tmp == '\v'){
            *tmp = '\0';
            --tmp;
            continue;
        }else{
            break;
        }
    }
    return new_buffer;
}

char * str_trim(const char * s, char * new_buffer){
    // trim the string s from both sides and return
    // the new string in new_buffer. the provided
    // buffer must be long enough
    if (!s || !new_buffer)
        return NULL;
    char * tmp = (char *)s;
    while (*tmp){
        if (*tmp == '\n' || *tmp == ' ' || *tmp == '\f' ||
            *tmp == '\t' || *tmp == '\r' || *tmp == '\v')
            ++tmp;
        else
            break;
    }
    // string in tmp is already ltrimmed
    str_rtrim(tmp, new_buffer);
    return new_buffer;
}

unsigned int str_count(const char * str, int ch){
    // counts the number of occurrence of ch in str
    // str must not be null
    int cnt = 0;
    char * tmp = (char*)str;
    while (*tmp){
        if (*tmp == ch)
            cnt++;
        tmp++;
    }
    return cnt;
}

unsigned int is_digit(int c){
    return c >= 0x30 && c <= 0x39?1:0;
}

unsigned int str_to_uint(const char * str){
    // convert str to unsigned integer
    // str must be positive integer less than 2^32-1
    int i = 0;
    int digit;
    char * tmp = (char*)str;
    while(*tmp){
        digit = (int)(*tmp) - 0x30;
        i = i * 10 + digit;
        tmp++;
    }
    return i;
}

