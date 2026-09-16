#include <stdlib.h>
#include <stdbool.h>

static int cmp_func(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

static void normalize(struct ListStruct *list) {
    if (!list || !list->head) return;
    struct ListNode *curr = list->head;
    struct ListNode *prev = NULL;

    while (curr) {
        while (curr->count < BLOCK_SIZE && curr->next) {
            struct ListNode *nxt = curr->next;
            if (nxt->count == 0) {
                curr->next = nxt->next;
                free(nxt);
                continue;
            }
            
            int space = BLOCK_SIZE - curr->count;
            int take = (nxt->count < space) ? nxt->count : space;

            for (int i = 0; i < take; i++) {
                curr->data[curr->count++] = nxt->data[i];
            }

            for (int i = 0; i < nxt->count - take; i++) {
                nxt->data[i] = nxt->data[i + take];
            }
            nxt->count -= take;

            if (nxt->count == 0) {
                curr->next = nxt->next;
                free(nxt);
            }
        }
        
        if (curr->count == 0) {
            if (prev) {
                prev->next = curr->next;
                free(curr);
                curr = prev->next;
            } else {
                list->head = curr->next;
                free(curr);
                curr = list->head;
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

struct ListStruct *list_init() {
    struct ListStruct *list = (struct ListStruct *)malloc(sizeof(struct ListStruct));
    if (list) list->head = NULL;
    return list;
}

void list_destroy(struct ListStruct *list) {
    if (!list) return;
    struct ListNode *curr = list->head;
    while (curr) {
        struct ListNode *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

bool list_is_empty(struct ListStruct *list) {
    return !list || !list->head;
}

int list_count(struct ListStruct *list) {
    if (!list) return 0;
    int count = 0;
    struct ListNode *curr = list->head;
    while (curr) {
        count += curr->count;
        curr = curr->next;
    }
    return count;
}

struct ListNode *list_get(struct ListStruct *list, int index, int *index_in_block) {
    if (!list) return NULL;
    struct ListNode *curr = list->head;
    while (curr) {
        if (index < curr->count) {
            if (index_in_block) *index_in_block = index;
            return curr;
        }
        index -= curr->count;
        curr = curr->next;
    }
    return NULL;
}

void list_push(struct ListStruct *list, int index, int data) {
    if (!list) return;
    int total = list_count(list);
    if (index < 0) index = 0;
    if (index > total) index = total;

    if (total == 0) {
        list->head = (struct ListNode *)calloc(1, sizeof(struct ListNode));
        list->head->data[0] = data;
        list->head->count = 1;
        return;
    }

    struct ListNode *curr = list->head;
    int curr_idx = index;

    while (curr) {
        if (curr_idx < curr->count) break;
        if (curr_idx == curr->count && curr->count < BLOCK_SIZE) break;
        
        curr_idx -= curr->count;
        
        if (curr_idx == 0 && !curr->next) {
            curr->next = (struct ListNode *)calloc(1, sizeof(struct ListNode));
            curr = curr->next;
            break;
        }
        curr = curr->next;
    }

    int carry = data;
    int insert_idx = curr_idx;

    while (curr) {
        int next_carry = 0;
        bool has_carry = false;

        if (curr->count == BLOCK_SIZE) {
            next_carry = curr->data[BLOCK_SIZE - 1];
            has_carry = true;
        } else {
            curr->count++;
        }

        for (int i = curr->count - 1; i > insert_idx; i--) {
            curr->data[i] = curr->data[i - 1];
        }
        curr->data[insert_idx] = carry;

        if (has_carry) {
            carry = next_carry;
            insert_idx = 0;
            if (!curr->next) {
                curr->next = (struct ListNode *)calloc(1, sizeof(struct ListNode));
                curr->next->data[0] = carry;
                curr->next->count = 1;
                break;
            }
            curr = curr->next;
        } else {
            break;
        }
    }

    normalize(list);
}

void list_push_front(struct ListStruct *list, int data) {
    list_push(list, 0, data);
}

void list_push_back(struct ListStruct *list, int data) {
    list_push(list, list_count(list), data);
}

void list_pop(struct ListStruct *list, int index) {
    if (!list || !list->head) return;
    int total = list_count(list);
    if (index < 0 || index >= total) return;

    struct ListNode *curr = list->head;
    int curr_idx = index;

    while (curr) {
        if (curr_idx < curr->count) break;
        curr_idx -= curr->count;
        curr = curr->next;
    }

    if (curr) {
        for (int i = curr_idx; i < curr->count - 1; i++) {
            curr->data[i] = curr->data[i + 1];
        }
        curr->count--;
    }

    normalize(list);
}

void list_sort(struct ListStruct *list) {
    int n = list_count(list);
    if (n <= 1) return;
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) return;

    struct ListNode *curr = list->head;
    int k = 0;
    while (curr) {
        for (int i = 0; i < curr->count; i++) arr[k++] = curr->data[i];
        curr = curr->next;
    }

    qsort(arr, n, sizeof(int), cmp_func);

    curr = list->head;
    k = 0;
    while (curr) {
        for (int i = 0; i < curr->count; i++) curr->data[i] = arr[k++];
        curr = curr->next;
    }
    free(arr);
    
    normalize(list);
}

void list_remove_duplicates(struct ListStruct *list) {
    if (!list || !list->head) return;
    int n = list_count(list);
    int *arr = (int *)malloc(n * sizeof(int));
    int *uniq = (int *)malloc(n * sizeof(int));
    if (!arr || !uniq) {
        free(arr); free(uniq); return;
    }

    struct ListNode *curr = list->head;
    int k = 0;
    while (curr) {
        for (int i = 0; i < curr->count; i++) arr[k++] = curr->data[i];
        curr = curr->next;
    }

    int u = 0;
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < u; j++) {
            if (uniq[j] == arr[i]) {
                found = true; break;
            }
        }
        if (!found) uniq[u++] = arr[i];
    }
    
    curr = list->head;
    int idx = 0;
    while (curr && idx < u) {
        int fill = (u - idx) > BLOCK_SIZE ? BLOCK_SIZE : (u - idx);
        curr->count = fill;
        for(int i = 0; i < fill; i++) {
            curr->data[i] = uniq[idx++];
        }
        if (idx == u) {
            struct ListNode *tmp = curr->next;
            curr->next = NULL;
            while (tmp) {
                struct ListNode *nxt = tmp->next;
                free(tmp);
                tmp = nxt;
            }
            break;
        }
        if (!curr->next && idx < u) {
            curr->next = (struct ListNode *)calloc(1, sizeof(struct ListNode));
        }
        curr = curr->next;
    }
    free(arr);
    free(uniq);
}

double list_median(struct ListStruct *list) {
    int n = list_count(list);
    if (n == 0) return 0.0;
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) return 0.0;

    struct ListNode *curr = list->head;
    int k = 0;
    while (curr) {
        for (int i = 0; i < curr->count; i++) arr[k++] = curr->data[i];
        curr = curr->next;
    }

    qsort(arr, n, sizeof(int), cmp_func);

    double med = 0.0;
    if (n % 2 != 0) {
        med = arr[n / 2];
    } else {
        med = (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    }
    free(arr);
    return med;
}

void list_reverse_blocks(struct ListStruct *list) {
    if (!list || !list->head || !list->head->next) return;
    
    struct ListNode *prev = NULL;
    struct ListNode *curr = list->head;
    
    while (curr) {
        struct ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}