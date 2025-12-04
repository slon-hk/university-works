char *read_arr();
char *join_arr(char **arr, size_t len_arr, char *sep);
char **split_sentence(char *str, size_t *count);
char *trim(char *s);
char *trim_punct(char *s);
int count_islower(const char *str);
void remove_sentence(char **arr, size_t *count, size_t index);