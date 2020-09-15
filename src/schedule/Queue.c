#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

Queue* init_queue(int pending_process)
{
  Queue* cola = malloc(sizeof(Queue));
  cola -> pending_processes     = pending_process;
  cola -> head_process_inactive = NULL;
  cola -> tail_process_inactive = NULL;
  cola -> head_process_ready    = NULL;
  cola -> tail_process_ready    = NULL;
  return cola;
}

void add_process_inactive(Queue* cola, Process* process)
{
  if (!(cola -> head_process_inactive)) 
  {
    cola -> head_process_inactive = process;
    cola -> tail_process_inactive = process;
    return;
  }

  if (process -> init_time <= cola -> head_process_inactive->init_time)
  {
    process -> next               = cola -> head_process_inactive;
    cola -> head_process_inactive = process;
    return;
  }

  if (process -> init_time >= cola -> tail_process_inactive->init_time)
  {
    cola -> tail_process_inactive -> next = process;
    cola -> tail_process_inactive         = process;
    return;
  }

  Process* last_process    = cola -> head_process_inactive;
  Process* current_process = last_process -> next;
  while (current_process)
  {
    if (process -> init_time <= current_process->init_time)
    {
      last_process -> next = process;
      process -> next      = current_process;
      return;
    }
    last_process    = current_process;
    current_process = current_process -> next;
  }
}

void add_process_ready(Queue* cola, Process* process)
{
  if (!(cola -> head_process_ready)) 
  {
    cola -> head_process_ready = process;
    cola -> tail_process_ready = process;
    return;
  }

  if (process -> deadline == cola -> head_process_ready -> deadline)
  {
    if (process -> pid < cola -> head_process_ready -> pid)
    {
      process -> next            = cola -> head_process_ready;
      cola -> head_process_ready = process;
      return;
    }
  }

  else if (process -> deadline < cola -> head_process_ready -> deadline)
  {
    process -> next            = cola -> head_process_ready;
    cola -> head_process_ready = process;
    return;
  }

  if (process -> deadline == cola -> tail_process_ready -> deadline)
  {
    if (process -> pid > cola -> tail_process_ready -> pid)
    {
      cola -> tail_process_ready -> next = process;
      cola -> tail_process_ready         = process;
      return; 
    }
  }

  else if (process -> deadline > cola -> tail_process_ready -> deadline)
  {
    cola -> tail_process_ready -> next = process;
    cola -> tail_process_ready         = process;
    return;
  }

  Process* last_process    = cola -> head_process_ready;
  Process* current_process = last_process -> next;
  while (current_process)
  {
    if (process -> deadline == current_process -> deadline)
    {
      if (process -> pid < current_process -> pid)
      {
        last_process -> next = process;
        process -> next      = current_process;
        return;
      }
    }

    else if (process -> deadline < current_process -> deadline)
    {
      last_process -> next = process;
      process -> next      = current_process;
      return;
    }

    last_process    = current_process;
    current_process = current_process -> next;
  }
}

void update_queue(Queue* cola, int time)
{
  Process* next_process;
  Process* current_process = cola->head_process_inactive;
  while(current_process)
  {
    if (current_process->init_time == time)
    {
      next_process           = current_process->next;
      current_process->next  = NULL;
      current_process->state = 1;
      add_process_ready(cola, current_process);
      current_process             = next_process;
      cola->head_process_inactive = current_process;
      continue;
    }
    break;
  } 
}

void Print(Queue *cola)
{
  Burst* tmp;
  Process* tmp_process;

  tmp_process = cola -> head_process_ready;

  while (tmp_process)
  {
    printf("Nombre Proceso: %s\n", tmp_process->nombre);
    printf("Pid: %i\n",            tmp_process->pid);
    printf("Inicio: %i\n",         tmp_process->init_time);
    printf("Deadline: %i\n",       tmp_process->deadline);
    printf("CPU_burst: %i\n",      tmp_process->CPU_burst);
    tmp = tmp_process-> burst_head;
    while (tmp)
    {
      printf("burst: %i\n", tmp -> burst_time);
      tmp = tmp -> next;
    }
    tmp_process = tmp_process -> next;
  } 
}

void destroy_bursts(Burst* current_burst)
{
  Burst* next_burst;
  while (current_burst)
  {
    next_burst = current_burst -> next;
    free(current_burst);
    current_burst = next_burst;
  }
  
}

void destroy_processes(Process* current_process)
{
  Process* next_process;
  while (current_process)
  {
    destroy_bursts(current_process -> burst_head);
    next_process = current_process -> next;
    free(current_process);
    current_process = next_process;
  }
}

void destroy_queue(Queue* cola)
{
  destroy_processes(cola -> head_process_inactive);
  destroy_processes(cola -> head_process_ready);
  free(cola);
}