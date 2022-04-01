#include <stdlib.h>
#include <string.h>
#include "child_process.h"
#include "utils.h"

int request_cs(const void * self) {
    process_information_t* processInformation =  (process_information_t*) self;
    push(&processInformation->pNode, get_lamport_time() + 1, processInformation->local_id);
    Message* request = create_message(NULL, CS_REQUEST);
    send_children(processInformation, request);
    while (processInformation->reply_count != processInformation->environment->processNum - 1) {
        receive_from_children(processInformation);
    }
    return 1;
}

int release_cs(const void * self) {
    process_information_t* processInformation = (process_information_t*) self;
    pop(&processInformation->pNode);
    processInformation->reply_count = 1;
    Message* release = create_message(NULL, CS_RELEASE);
    send_children(processInformation, release);
    return 1;
}

void print_without_mutex(process_information_t* processInformation) {
    int N = processInformation->local_id * 5;

    for (int i = 1; i <= N; i ++) {
        char* payload = malloc(strlen(log_loop_operation_fmt));
        sprintf(payload, log_loop_operation_fmt, processInformation->local_id, i, N);
        print(payload);
        free(payload);
    }
}

void print_with_mutex(process_information_t* processInformation) {

    int N = processInformation->local_id * 5;

    for (int i = 1; i <= N; i++) {
        request_cs(processInformation);
        while (
                get_live_process_count(processInformation->liveProcesses, processInformation->environment->processNum) != 1
                && (*processInformation->pNode).local_id != processInformation->local_id) {
            receive_from_children(processInformation);
        }
        char* payload = malloc(strlen(log_loop_operation_fmt));
        sprintf(payload, log_loop_operation_fmt, processInformation->local_id, i, N);
        print(payload);
        free(payload);
        release_cs(processInformation);
    }

}

void work_stage_child_process(process_information_t* processInformation) {

    close_unused_pipes(processInformation);

    char* payload = print_output(log_started_fmt, processInformation, STARTED);
    Message* message = create_message(payload, STARTED);
    send_multicast((void*) processInformation, message);
    free(payload);
    free(message);

    while (processInformation->environment->processNum - 1 !=
        get_live_process_count(processInformation->liveProcesses, processInformation->environment->processNum)) {
        receive_from_children(processInformation);
    }

    payload = print_output(log_received_all_started_fmt, processInformation, ANY_TYPE);
    free(payload);

    if (processInformation->environment->mutex_mode == 1) {
        print_with_mutex(processInformation);
    } else {
        print_without_mutex(processInformation);
    }


    payload = print_output(log_done_fmt, processInformation, DONE);
    message = create_message(payload, DONE);
    send_multicast(processInformation, message);
    free(payload);
    free(message);

    while (get_live_process_count(processInformation->liveProcesses, processInformation->environment->processNum) != 1) {
        receive_from_children(processInformation);
    }

    payload = print_output(log_received_all_done_fmt, processInformation, ANY_TYPE);
    free(payload);

    exit(0);
}
