#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "slon_tools.h"

char *read_arr(){
    size_t size = 64;
    size_t len = 0;
    char *arr = malloc(size);
    if (!arr) return NULL;

    int ch;
    while ((ch = getchar()) != EOF) {
        if (len + 1 >= size) {
            size *= 2;
            char *tmp = realloc(arr, size);
            if (!tmp) {
                free(arr);
                return NULL;
            }
            arr = tmp;
        }

        arr[len++] = (char)ch;


        if (len >= 2 && arr[len-1] == '\n' && arr[len-2] == '\n') {
            len -= 2;
            break;
        }
    }

    arr[len] = '\0';
    return arr;
}

char *join_arr(char **arr, size_t len_arr, char *sep) {
    if (len_arr == 0) {
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    size_t total_len = 0;
    size_t sep_len = strlen(sep);

    for (size_t i = 0; i < len_arr; i++) {
        total_len += strlen(arr[i]);
    }
    total_len += sep_len * (len_arr - 1);
    total_len += 1; 

    char *res = malloc(total_len);
    if (!res) return NULL;

    char *p = res;
    for (size_t i = 0; i < len_arr; i++) {
        size_t len = strlen(arr[i]);
        memcpy(p, arr[i], len);
        p += len;

        if (i < len_arr - 1 && sep_len > 0) {
            memcpy(p, sep, sep_len);
            p += sep_len;
        }
    }

    *p = '\0';
    return res;
}

char **split_sentence(char *str, size_t *count){
    size_t arr_size = 8;
    size_t len = 0;
    char **arr = malloc(arr_size * sizeof(char*));
    if (!arr) return NULL;

    char *copy = strdup(str);
    if(!copy){
        free(arr);
        return NULL;
    }

    char *myPtr = strtok(copy, ".");
    while(myPtr != NULL){
        // Пропускаем пробелы в начале
        while(*myPtr == ' ' || *myPtr == '\t' || *myPtr == '\n' || *myPtr == '\r'){
            myPtr++;
        }

        // Пропускаем пустые строки
        size_t myLen = strlen(myPtr);
        if(myLen == 0){
            myPtr = strtok(NULL, ".");
            continue;
        }

        if(len >= arr_size){
            arr_size *= 2;
            char **tmp = realloc(arr, arr_size * sizeof(char*));
            if(!tmp){
                for(size_t i = 0; i < len; i++) free(arr[i]);
                free(arr);
                free(copy);
                return NULL;
            }
            arr = tmp;
        }

        arr[len] = malloc(myLen + 2);
        if(!arr[len]){
            for(size_t i = 0; i < len; i++) free(arr[i]);
            free(arr);
            free(copy);
            return NULL;
        }

        strcpy(arr[len], myPtr);
        arr[len][myLen] = '.';  
        arr[len][myLen+1] = '\0';

        len++;
        myPtr = strtok(NULL, ".");
    }

    free(copy);
    *count = len;  
    return arr;   
}

char *trim(char *s) {
    while(isspace((char)*s)) s++;
    if(*s == 0) return s;

    char *end = s + strlen(s) - 1;
    while(end > s && isspace((char)*end)) end--;

    *(end+1) = '\0';
    return s;
}

char *trim_punct(char *s) {
    while(ispunct((char)*s)) s++;
    if(*s == 0) return s;

    char *end = s + strlen(s) - 1;
    while((end > s && ispunct((char)*end))) end--;

    *(end+1) = '\0';
    return s;
}

void remove_sentence(char **arr, size_t *count, size_t index){
    if(index >= *count) return;

    free(arr[index]);
    for(size_t i = index; i < *count - 1; i++){
        arr[i] = arr[i+1];
    }
    (*count)--;
}

void swap(char **arr1, char **arr2){
    char *buffer = *arr1;
    *arr1 = *arr2;
    *arr2 = buffer;
}

int count_islower(const char *str) {
    int counter = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (islower((unsigned char)str[i])) counter++;
    }
    return counter;
}