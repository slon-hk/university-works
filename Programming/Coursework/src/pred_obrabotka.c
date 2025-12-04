#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "pred_obrabotka.h"
#include "slon_tools.h"

char *pred_obrabotka(char *str){
    for (char *p = str; *p; p++) {
        if (*p == '\n' || *p == '\r') *p = ' ';
    }

    size_t len;
    char **arr = split_sentence(str, &len);
    if (!arr) return NULL;

    for(size_t i = 0; i < len; i++){
        for(size_t j = i + 1; j < len; j++){
            if(strcasecmp(arr[i], arr[j]) == 0){
                remove_sentence(arr, &len, j);
                j--;
            }
        }
    }

    char *arr_done = join_arr(arr, len, "\n");

    for(size_t i = 0; i < len; i++) free(arr[i]);
    free(arr);

    return arr_done;
}