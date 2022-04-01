#ifndef PA1_UTILS_H
#define PA1_UTILS_H

#include "general_structures.h"
#include "malloc.h"
#include "pa2345.h"
#include "banking.h"
#include "ipc.h"

#define ANY_TYPE -1

void increment_lamport_time();
process_information_t* create_process(environment_t* environment, int local_id);
void set_lamport_time(timestamp_t timestamp);
void close_unused_pipes(process_information_t* processInformation);
Message* create_message(const char* payload, MessageType type);
void set_default_state_live_processes(const int local_id, int* pProcesses, int process_count);
char* print_output(const char* message_format, const process_information_t* processInformation, MessageType type);
int get_live_process_count(int* processes, int process_count);
void receive_from_children(process_information_t* processInformation);
environment_t* init_environment(int process_num, int is_mutex_mode);
void send_children(process_information_t* processInformation, Message* message);
#endif //PA1_UTILS_H
