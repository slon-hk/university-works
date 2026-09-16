#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "slon_tools.h"
#include "three.h"

char *remove_equal_length_sentences(char *str) {
    if (!str) return NULL;

    for (char *p = str; *p; p++) {
        if (*p == '\n' || *p == '\r') *p = ' ';
    }

    size_t len;
    char **arr = split_sentence(str, &len);
    if (!arr) return NULL;

    size_t i = 0;
    while (i < len) {
        char *sentence = strdup(arr[i]); 
        if (!sentence) {
            i++;
            continue;
        }

        size_t slen = strlen(sentence);
        if (slen > 0 && sentence[slen-1] == '.') {
            sentence[slen-1] = '\0';
        }

        char *saveptr;
        char *token = strtok_r(sentence, " \t", &saveptr);
        int all_equal = 1;
        size_t first_len = 0;
        int word_count = 0;

        while (token != NULL) {
            size_t wlen = 0;
            for (char *p = token; *p; p++) {
                if (isalpha((unsigned char)*p)) {
                    wlen++;
                }
            }
            
            if (wlen > 0) {
                if (word_count == 0) {
                    first_len = wlen;
                } else if (wlen != first_len) {
                    all_equal = 0;
                }
                word_count++;
            }
            
            token = strtok_r(NULL, " \t", &saveptr);
        }

        free(sentence);

        if (all_equal && word_count > 1) {
            remove_sentence(arr, &len, i);
        } else {
            i++;
        }
    }

    char *result = join_arr(arr, len, "\n");

    for (size_t j = 0; j < len; j++)
        free(arr[j]);
    free(arr);

    return result;
}