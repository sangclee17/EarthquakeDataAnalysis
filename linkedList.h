#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* node type */
typedef struct node node_t;

struct node {
    void* data;
    node_t* next;
};

/* linked list type */
typedef struct {
    int num_elements;
    node_t* head;
    node_t* tail;
    void (*del)(void*);
} list_t;

typedef struct {
    eq_t* x;
    double score;
} score_t;

void list_process_all(node_t* p, void (*process)(void*));
void* list_pop_front(list_t* list);
void list_push_front(list_t* list, void* d);
void list_push_back(list_t* list, void* d);
list_t* list_new(void (*delfunc)(void*));
void list_free(list_t* list);
void list_insert_in_order(list_t* list, void* d, int cmp(const void*, const void*));

double compute_score(list_t* list);
int score_cmp(const void* a, const void* b);
float find_median(list_t* list);
int magnitude_cmp(const void* a, const void* b);

#endif
