#pragma once

#include "Process.h"

typedef struct queue Queue;

struct queue
{
  int      pending_processes;
  Process* head_process_inactive;
  Process* tail_process_inactive;
  Process* head_process_ready;
  Process* tail_process_ready;
};

Queue* init_queue();
void   add_process_inactive(Queue* cola, Process* process);
void   update_queue(Queue* cola, int time);
void   Print(Queue *cola);
void   destroy_queue(Queue* cola);