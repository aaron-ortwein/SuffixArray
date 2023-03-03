#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

struct bst* bst_insert(struct bst* root, void* data, int (*cmp)(void*, void*)) {
    if (!root) {
        struct bst* node = malloc(sizeof(struct bst));
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    else if (cmp(data, root->data)) root->left = bst_insert(root->left, data, cmp);
    else                            root->right = bst_insert(root->right, data, cmp);

    return root;
}

void bst_free(struct bst* root, void (*free_data)(void*)) {
    if (!root) return;
    if (root->data) free_data(root->data);
    bst_free(root->left, free_data);
    bst_free(root->right, free_data);
    free(root);
}
