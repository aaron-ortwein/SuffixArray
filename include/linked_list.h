struct node {
    void* data;
    struct node* next;
};

struct linked_list {
    struct node *head, *tail;
};

struct linked_list* initialize_linked_list();
void append(struct linked_list* list, void* data);
void free_linked_list(struct linked_list* list, void (*free_data)(void*));