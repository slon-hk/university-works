#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "two.h"
#include "slon_tools.h"

char *del_first_sentence(char *str){
    size_t len;
    char **arr = split_sentence(str, &len);

    for (size_t i = 0; i < len; i++) {
        char *s = arr[i];
        if (strchr(s, ' ') == NULL) {
            memmove(&arr[i], &arr[i+1], (len - i - 1) * sizeof(char*));
            len--;
            i--;
            continue;
        }
        for (size_t y = 0; s[y] != '\0'; y++) {
            if (s[y] == ' ') {
                memmove(s, s + y + 1, strlen(s + y + 1) + 1);
                if(*trim(s) == '\0'){
                    memmove(&arr[i], &arr[i+1], (len - i - 1) * sizeof(char*));
                }
                break;
            }
        }
    }

    size_t size = 64;
    size_t len_done = 0;
    char *arr_done = malloc(size);
    if(!arr_done) return NULL;

    for(size_t i = 0; i < len; i++){
        size_t slen = strlen(arr[i]);

        while(len_done + slen + 2 > size){
            size *= 2;
            char *tmp = realloc(arr_done, size);
            if(!tmp){
                free(arr_done);
                return NULL;
            }
            arr_done = tmp;
        }

        memcpy(arr_done + len_done, arr[i], slen);
        len_done += slen;

        if(i + 1 < len && arr[i][slen - 1] == '.'){
            arr_done[len_done++] = '\n';
        }
    }

    arr_done[len_done] = '\0';

    for(size_t i = 0; i < len; i++) free(arr[i]);
    free(arr);

    return arr_done;
}