#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "four.h"
#include "slon_tools.h"

typedef struct {
    char *str;
    int original_index;
    int lower_count;
} SentenceInfo;

int compare_sentences(const void *a, const void *b) {
    SentenceInfo *sa = (SentenceInfo *)a;
    SentenceInfo *sb = (SentenceInfo *)b;

    if (sb->lower_count != sa->lower_count) {
        return sb->lower_count - sa->lower_count;
    }
    return sa->original_index - sb->original_index;
}

static void remove_newlines(char *s) {
    for (char *p = s; *p; p++) {
        if (*p == '\n' || *p == '\r')
            *p = ' ';
    }
}

char *sort_by_islower(char *str){
    if (!str) return NULL;

    remove_newlines(str);

    size_t len = 0;
    char **arr = split_sentence(str, &len);
    if (!arr) return NULL;

    SentenceInfo *info = malloc(len * sizeof(SentenceInfo));
    if (!info) {
        for(size_t k=0; k<len; k++) free(arr[k]);
        free(arr);
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        info[i].str = arr[i];
        info[i].original_index = (int)i;
        info[i].lower_count = count_islower(arr[i]);
    }

    qsort(info, len, sizeof(SentenceInfo), compare_sentences);

    for (size_t i = 0; i < len; i++) {
        arr[i] = info[i].str;
    }

    char *result = join_arr(arr, len, "\n");

    free(info);
    for (size_t i = 0; i < len; i++)
        free(arr[i]);
    free(arr);

    return result;
}