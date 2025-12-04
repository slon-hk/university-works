#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STOP_PH "Dragon flew away!"

int check_sentence(char *sen);

int main(){
    int c_pred = 0;
    int c_pred_con = 0;

    int size_arr = 1;
    int len_arr = 0;
    char **arr = (char **)malloc(size_arr * sizeof(char *));
    
    int c;
    char *text = NULL;
    int len = 0, size = 16;
    text = malloc(size);

    while(1){
        c = getchar();
        if (c == EOF) break;
        if (c == '\n' && len == 0) continue;
        if (c == '\t' && len == 0) continue;
        if (c == ' ' && len == 0) continue;
        if (c == '\n') c = ' ';

        if(len+1>=size){
            size*=2;
            char *tmp = realloc(text, size);
            if(!tmp){
                free(text);
                free(arr);
                return 1;
            }
            text = tmp;
        }

        text[len++] = c;

        if (c == '.' || c == ';' || c == '?' || c == '!'){
            text[len] = '\0';
            char *trim = text;
            while (*trim == ' ' || *trim == '\t') trim++;
            if (strcmp(trim, STOP_PH) == 0) break;
            c_pred++;

            if (!check_sentence(trim)) {
                c_pred_con++; 
                if (len_arr >= size_arr){
                    size_arr*=2;
                    char **tmp_arr = realloc(arr, size_arr*sizeof(char *));
                    if (!tmp_arr){
                        free(arr);
                        free(text);
                        return 1;
                    }
                    arr = tmp_arr;
                }
                arr[len_arr++] = strdup(trim);
            }
            text = malloc(16);
            size = 16;
            len = 0;
        }
    }

    printf("\n");
    for (int i = 0; i < len_arr; i++) {
        printf("%s\n", arr[i]);
    }
    printf("%s\n", STOP_PH);
    printf("Количество предложений до %d и количество предложений после %d\n", c_pred, c_pred_con);
    
    for (int i = 0; i < len_arr; i++) {
        free(arr[i]);
    }
    free(arr);
    free(text);
    
    return 0;
}

int check_sentence(char *sen) {
    char *temp = strdup(sen);
    if (!temp) return 0;
    char *word = strtok(temp, " \t\n");
    while (word != NULL) {
        int len = strlen(word);
        int first_al = -1, last_al = -1;
        for (int i = 0; i < len; i++) {
            if (isalnum((unsigned char)word[i])) {
                if (first_al == -1) first_al = i;
                last_al = i;
            }
        }
        if (first_al != -1) {
            int has_letter = 0;
            for (int i = first_al; i <= last_al; i++) {
                if (isalpha((unsigned char)word[i])) { has_letter = 1; break; }
            }
            if (has_letter) {
                for (int i = first_al + 1; i <= last_al - 1; i++) {
                    if (isdigit((unsigned char)word[i])) {
                        free(temp);
                        return 1;
                    }
                }
            }
        }
        word = strtok(NULL, " \t\n");
    }
    free(temp);
    return 0;
}
