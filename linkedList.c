#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "eq_util.h"
#include "linkedlist.h"

/* process all elements in the list */
void list_process_all(node_t* p, void (*process)(void*))
{
    while (p) {
        process(p->data);
        p = p->next;
    }
}

/* remove node at the front of the list */
void* list_pop_front(list_t* list)
{
    assert(list != NULL);
    assert(list->num_elements > 0);
    node_t* old;
    assert(list->head != NULL);
    old = list->head;
    list->head = list->head->next;
    void* d = old->data;
    free(old);
    list->num_elements--;
    if (list->num_elements == 0) {
        list->head = NULL;
        list->tail = NULL;
    }
    return d;
}

/* add node add the front of the list */
void list_push_front(list_t* list, void* d)
{
    assert(list != NULL);
    node_t* new = (node_t*)safe_malloc(sizeof(node_t));
    assert(new);
    new->data = d;
    new->next = list->head;
    list->head = new;
    if (list->tail == NULL)
        list->tail = new;
    list->num_elements++;
}

/* add node add the back of the list */
void list_push_back(list_t* list, void* d)
{
    assert(list != NULL);
    node_t* new = (node_t*)safe_malloc(sizeof(node_t));
    assert(new != NULL);
    new->data = d;
    new->next = NULL;
    if (list->tail)
        list->tail->next = new;
    list->tail = new;
    if (list->head == NULL)
        list->head = new;
    list->num_elements++;
}

void list_insert_after(list_t* list, node_t* n, void* d)
{
    assert(list != NULL);
    if (n == list->tail) {
        list_push_back(list, d);
    } else {
        node_t* new = (node_t*)safe_malloc(sizeof(node_t));
        assert(new != NULL);
        new->data = d;
        new->next = n->next;
        n->next = new;
        list->num_elements++;
    }
}

void list_insert_in_order(list_t* list, void* d, int cmp(const void*, const void*))
{
    if (list->head == NULL) {
        list_push_back(list, d);
        return;
    }
    if (cmp(list->head->data, d) < 0) {
        list_push_front(list, d);
        return;
    }

    node_t* tmp = list->head;
    while (tmp->next) {
        if (cmp(tmp->next->data, d) < 0) {
            break;
        }
        tmp = tmp->next;
    }
    list_insert_after(list, tmp, d);
}

/* create a new linked list structure */
list_t* list_new(void (*delfunc)(void*))
{
    list_t* list;
    list = (list_t*)safe_malloc(sizeof(list_t));
    assert(list != NULL);
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
    list->del = delfunc;
    return list;
}

int magnitude_cmp(const void* a, const void* b)
{
    eq_t* temp_a = (eq_t*)a;
    eq_t* temp_b = (eq_t*)b;

    float mag_cmp = (temp_a->magnitude - temp_b->magnitude);
    if (mag_cmp < 0)
        return -1;
    if (mag_cmp > 0)
        return 1;
    return 0;
}

double compute_score(list_t* list)
{
    double sum = 0;
    if (list == NULL)
        return sum;
    node_t* temp = list->head;
    for (int i = 0; i <= (list->num_elements) / 2; i++) {
        eq_t* eq_data = temp->data;
        sum = sum + pow(31.62278, eq_data->magnitude);
        temp = temp->next;
    }
    return sum;
}

int score_cmp(const void* a, const void* b)
{
    score_t* score_a = (score_t*)a;
    score_t* score_b = (score_t*)b;

    double score_diff = (score_b->score - score_a->score);
    if (score_diff < 0)
        return -1;
    if (score_diff > 0)
        return 1;
    return 0;
}

/* free all memory associated with a list */
void list_free(list_t* list)
{
    assert(list != NULL);
    while (list->num_elements) {
        void* elem = list_pop_front(list);
        list->del(elem);
    }
    free(list);
}
