#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct linked_list* initialize_linked_list() {
    struct linked_list* list = malloc(sizeof(struct linked_list));

    struct node* dummy = malloc(sizeof(struct node));
    dummy->data = NULL;
    dummy->next = NULL;

    list->tail = list->head = dummy;

    return list;
}

void append(struct linked_list* list, void* data) {
    struct node* node = malloc(sizeof(struct node));
    node->data = data;
    node->next = NULL;

    list->tail->next = node;
    list->tail = node;
}

void free_linked_list(struct linked_list* list, void (*free_data)(void*)) {
    struct node* current = list->head;

    while (current) {
        struct node* next = current->next;
        if (current->data) free_data(current->data);
        free(current);
        current = next;
    }

    free(list);
}
