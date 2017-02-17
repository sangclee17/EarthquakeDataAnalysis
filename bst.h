#ifndef BST_H
#define BST_H

#define BST_SUCCESS  1
#define BST_FAILURE  0

#define BST_PREORDER  0
#define BST_INORDER   1
#define BST_POSTORDER 2

/* node type */
typedef struct bst_node bst_node_t;

struct bst_node {
    void* data;
    bst_node_t* left;
    bst_node_t* right;
};

/* bst type */
typedef struct {
    int num_elements;
    bst_node_t* root;
    void (*del)(void*);
    int (*cmp)(void*,void*);
} bst_t;

/* create and free */
bst_t* bst_new(void (*delfunc)(void*),int (*cmpfunc)(void*,void*));
void bst_free(bst_t* bst);
void bst_free_subtree(bst_t* bst,bst_node_t* n);

/* insert and remove */
int bst_insert(bst_t* bst,void* d);
int bst_remove(bst_t* bst,void* d);

/* find */
int bst_contains(bst_t* bst,void* d);
bst_node_t* bst_find(bst_t* bst,void* d);
bst_node_t* bst_recursive_find(bst_t* bst,bst_node_t* n,void* d);

/* traversal */
void bst_traverse(bst_t* bst,int BST_ORDER,void (*process)(void*));

/* advanced stuff */
bst_node_t* bst_successor(bst_t* bst,void* d);
bst_node_t* bst_min(bst_t* bst);
bst_node_t* bst_max(bst_t* bst);

/* helper functions */
void bst_traverse_preorder(bst_node_t* n,void (*process)(void*));
void bst_traverse_inorder(bst_node_t* n,void (*process)(void*));
void bst_traverse_postorder(bst_node_t* n,void (*process)(void*));
void bst_print(bst_node_t* n,void (*print)(void*,void*));

#endif
