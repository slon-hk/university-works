#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *get_text();

int main() {
    char *str = get_text();
    if (!str) return 1;

    regex_t regex;
    regmatch_t matches[6]; 
    
    char *pattern = "([a-zA-Z]+://)?(www\\.)?([^/ \n\r]+)(/[^ \n\r]*)*/([^/ \n\r]+\\.[a-zA-Z0-9]+)";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        free(str);
        return 1;
    }

    char *cursor = str;
    while (regexec(&regex, cursor, 6, matches, 0) == 0) {
        regoff_t s_start = matches[3].rm_so;
        regoff_t s_end = matches[3].rm_eo;
        
        regoff_t f_start = matches[5].rm_so;
        regoff_t f_end = matches[5].rm_eo;

        for (regoff_t i = s_start; i < s_end; i++) {
            putchar(cursor[i]);
        }

        printf(" - ");

        for (regoff_t i = f_start; i < f_end; i++) {
            putchar(cursor[i]);
        }
        printf("\n");

        cursor += matches[0].rm_eo;
    }

    regfree(&regex);
    free(str);
    
    return 0;
}

char *get_text() {
    size_t cap = 128;
    size_t len = 0;
    char* str = malloc(cap);
    if (!str) return NULL;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (len + 1 >= cap) {
            cap *= 2;
            char* rel = realloc(str, cap);
            if (!rel) {
                free(str);
                return NULL;
            }
            str = rel;
        }

        str[len++] = (char)c;
        str[len] = '\0';

        if (len >= 4 && strcmp(&str[len - 4], "Fin.") == 0) {
            break;
        }
    }
    return str;
}