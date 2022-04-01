#ifndef PA1_QUEUE_UTILS_H
#define PA1_QUEUE_UTILS_H

#include "malloc.h"

typedef struct node {
    int local_time;
    int local_id;
    struct node* next;
} node_t;

void pop(node_t** pNode);
void push(node_t** pNode, int local_time, int local_id);

#endif //PA1_QUEUE_UTILS_H
