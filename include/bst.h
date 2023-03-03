struct bst {
    void* data;
    struct bst* left;
    struct bst* right;
};

struct bst* bst_insert(struct bst* root, void* data, int (*cmp)(void*, void*));
void bst_free(struct bst* root, void (*free_data)(void*));