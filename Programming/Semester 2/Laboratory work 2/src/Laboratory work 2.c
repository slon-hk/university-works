#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct ListNode {
    struct ListNode *next;  
    int data;  
};

struct ListStruct {
    struct ListNode *head;
};

struct ListStruct *list_init() {
    struct ListStruct *list = malloc(sizeof(struct ListStruct));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void list_destroy(struct ListStruct *list) {
    if (list == NULL) {
        return;
    }

    struct ListNode *buff = list->head;
    struct ListNode *tmp;

    while(buff != NULL){
        tmp = buff->next;
        free(buff);
        buff = tmp;
    }
}

void list_push_front(struct ListStruct *list, int data) {
    struct ListNode *tmp = malloc(sizeof(struct ListNode));
    if (tmp == NULL) {
        return;
    }
    tmp->data = data;
    tmp->next = list->head;
    list->head = tmp;
}

void list_push_back(struct ListStruct *list, int data) {
    struct ListNode *tmp = malloc(sizeof(struct ListNode));
    if (tmp == NULL) {
        return;
    }
    tmp->data = data;
    tmp->next = NULL;

    if (list->head == NULL) {
        list->head = tmp;
        return;
    }
    
    struct ListNode *buff = list->head;
    while(buff->next != NULL){
        buff = buff->next;
    }
    buff->next = tmp;
}

void list_push(struct ListStruct *list, struct ListNode *prev_node, int data) {
    if (list == NULL) {
        return;
    }
    struct ListNode *tmp = malloc(sizeof(struct ListNode));
    if (tmp == NULL) {
        return;
    }
    tmp->data = data;
    tmp->next = NULL;

    if (prev_node == NULL) {
        if (list->head == NULL) {
            list->head = tmp;
            return;
        }
        struct ListNode *cur = list->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = tmp;
        return;
    }
    tmp->next = prev_node->next;
    prev_node->next = tmp;
}

// void list_pop(struct ListStruct *list, struct ListNode *node_to_remove) {
//     if(list == NULL && node_to_remove == NULL){
//         return;
//     }
//     struct ListNode *buf = list->head;
//     while(buf->next != NULL) {

//     }
// }

int list_count(struct ListStruct *list) {
    if(list == NULL){
        return 0;
    }
    struct ListNode *buf = list->head;
    int count = 0;
    while(buf != NULL){
        buf = buf->next;
        count++;
    }
    return count;
}

struct ListNode *list_get(struct ListStruct *list, int index) {
    
}

struct ListNode *list_get_head(struct ListStruct *list) {
    
}

struct ListNode *list_get_tail(struct ListStruct *list) {
    
}

// bool list_is_empty(struct ListStruct *list) {
    
// }

void list_sort(struct ListStruct *list) {
    
}