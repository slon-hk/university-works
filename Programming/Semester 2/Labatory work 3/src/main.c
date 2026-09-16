#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

struct Data {
    long long id;
    char line[1000];
} arr[10000];

int count = 0;

void find_files(const char *dir_path) {
    DIR *d = opendir(dir_path);
    if (d == NULL) return;

    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
        if (strcmp(dir->d_name, "result.txt") == 0) continue;

        char path[2048];
        snprintf(path, sizeof(path), "%s/%s", dir_path, dir->d_name);

        struct stat st;
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                find_files(path);
            } else {
                int len = strlen(dir->d_name);
                if (len > 4 && strcmp(dir->d_name + len - 4, ".txt") == 0) {
                    FILE *f = fopen(path, "r");
                    if (f) {
                        char buf[1000];
                        if (fgets(buf, 1000, f)) {
                            long long val;
                            if (sscanf(buf, "%lld", &val) == 1) {
                                arr[count].id = val;
                                buf[strcspn(buf, "\r\n")] = 0;
                                strcpy(arr[count].line, buf);
                                count++;
                            }
                        }
                        fclose(f);
                    }
                }
            }
        }
    }
    closedir(d);
}

int cmp(const void *a, const void *b) {
    struct Data *x = (struct Data *)a;
    struct Data *y = (struct Data *)b;
    if (x->id < y->id) return -1;
    if (x->id > y->id) return 1;
    return 0;
}

int main() {
    find_files(".");
    
    qsort(arr, count, sizeof(struct Data), cmp);
    
    FILE *out = fopen("result.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(out, "%s\n", arr[i].line);
    }
    fclose(out);
    
    return 0;
}