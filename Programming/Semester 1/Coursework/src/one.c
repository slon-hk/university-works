#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "one.h"

void print_sum_time(char *text){
    long long sum_sec = 0;

    char *pos = text;
    char *needle = "sec";

    while ((pos = strstr(pos, needle)) != NULL) {
        int end = (int)(pos - text) - 1;
        int start = end;

        while (start >= 0 && isdigit(text[start])){
            start--;
        }
        start++;

        if (start <= end){
            char num_buf[64];
            int len = end - start + 1;
            if (len > 0 && len < 63) {
                strncpy(num_buf, text + start, len);
                num_buf[len] = '\0';
                
                long val = atol(num_buf);
                sum_sec += val;
                
                long h = val / 3600;
                long m = (val % 3600) / 60;
                long s = val % 60;
                printf("%ld:%ld:%ld\n", h, m, s);
            }
        }
        pos += 3; 
    }
}