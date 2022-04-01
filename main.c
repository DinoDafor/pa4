#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "general_structures.h"
#include "utils.h"
#include "child_process.h"
#include "string.h"

#define MUTEX_ARG "--mutexl"
#define PROCESSES_ARG "-p"

static timestamp_t current_time = 0;

timestamp_t get_lamport_time() {
    return current_time;
}

void increment_lamport_time() {
    current_time ++;
}

void set_lamport_time(timestamp_t timestamp) {
    current_time = timestamp > current_time ? timestamp : current_time;
}


void work_stage_main_process(process_information_t* processInformation) {

    close_unused_pipes(processInformation);
    while (waitpid(-1, NULL, 0) != -1);

}

int main(int argc, char* argv[]) {
    int process_count;
    int is_mutex_mode = 0;

    for (int i = 0; i < argc; i ++) {
        if (strcmp(argv[i], MUTEX_ARG) == 0) {
            is_mutex_mode = 1;
        } else if (strcmp(argv[i], PROCESSES_ARG) == 0) {
            sscanf(argv[++i], "%d", &process_count);
        }
    }


    environment_t* environment = init_environment(process_count + 1, is_mutex_mode);

    __pid_t process;
    for (int process_id = 1; process_id < environment->processNum; process_id ++) {
        if ((process = fork()) == 0) {
            process_information_t* processInformation = create_process(environment, process_id);
            work_stage_child_process(processInformation);
        }
    }

    process_information_t* processInformation = create_process(environment, 0);

    work_stage_main_process(processInformation);

    close(processInformation->environment->fdLog);


    return 0;
}
