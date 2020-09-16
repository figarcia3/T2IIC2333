#pragma once

#include "Process.h"
#include "CPU.h"

typedef struct queue Queue;

struct queue
{
  int      pending_processes;
  Process* head_process_inactive;
  Process* tail_process_inactive;
  Process* head_process_ready;
  Process* tail_process_ready;
  Process* head_process_waiting;
  Process* tail_process_waiting;
  Process* head_process_finished;
  Process* tail_process_finished;
};

Queue* init_queue();
void   add_process_inactive(Queue* cola, Process* process);
void   add_process_ready(Queue* cola, Process* process);
void   add_process_finished(Queue* cola, Process* process);
void   add_process_waiting(Queue* cola, Process* process);

void   update_waiting(Queue* cola);
void   update_inactive(Queue* cola, int time);
void   update_ready(Queue* cola, CPU* cpu);
void   update_cpu(Queue* cola, CPU* cpu);
void   destroy_queue(Queue* cola);