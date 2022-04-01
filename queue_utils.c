#include "queue_utils.h"


node_t* create_node(int local_time, int local_id) {
    node_t* new_node = malloc(sizeof(node_t));

    new_node->local_id = local_id;
    new_node->local_time = local_time;
    new_node->next = NULL;

    return new_node;
}


void pop(node_t** pNode) {
    node_t* deleted_node = *pNode;
    (*pNode) = (*pNode)->next;
    free(deleted_node);
}

void push(node_t** pNode, int local_time, int local_id) {
    node_t* head = *pNode;

    node_t* new_node = create_node(local_time, local_id);

    if (head == NULL) {
        new_node->next = *pNode;
        (*pNode) = new_node;
    } else {
        if ((*pNode)->local_time > local_time || ((*pNode)->local_time == local_time && (*pNode)->local_id > local_id)) {
            new_node->next = *pNode;
            (*pNode) = new_node;
        } else {
            while (head->next != NULL && (head->next->local_time < local_time || (head->next->local_time == local_time && head->next->local_id < local_id))) {
                head = head->next;
            }
            new_node->next = head->next;
            head->next = new_node;
        }
    }
}
