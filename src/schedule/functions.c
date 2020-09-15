#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void FreeList(Burst* head) // Bibliografia
{
  Burst* tmp;
  while (head)
  {
      tmp = head;
      head = head->next;
      free(tmp);
  }
}
/*
void FreeMem(Queue *cola, int total_process)
{
  for (int i = 0; i < total_process; i++) // Libero memoria arreglo.
  {
    FreeList(cola->array[i]->burst_head); // liberando burst
    free(cola->array[i]);// liberando Proceso
  }
  free(cola -> array); // liberando Arreglo
  free(cola); // liberando cola
}*/

void Print(Queue *cola)
{
  Burst* tmp;
  Process* tmp_process;

  tmp_process = cola -> head_process_inactive;

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

void add_burst(Process *proceso, int time) // Agrega un Burst al final de la cola.
{
  Burst *burst = (Burst*)malloc(sizeof(Burst));
  burst -> next = NULL;
  if (proceso -> burst_head)
  {
    if (proceso -> burst_tail -> type == 0) // Si el ultimo burst fue de CPU, quiere decir que el siguiente es de IO
    {
      burst -> type = 1;
    }
    else
    {
      burst -> type = 0;
    }
    burst -> burst_time = time;
    proceso -> burst_tail -> next = burst;
    proceso -> burst_tail = burst;
  }
  else
  {
    burst -> type = 0; // 0 equivale que es de CPU.
    burst -> burst_time = time;
    proceso -> burst_head = burst;
    proceso -> burst_tail = burst;
  }
}

Queue* init_queue()
{
  Queue* cola = malloc(sizeof(Queue));
  cola -> head_process_inactive = NULL;
  cola -> tail_process_inactive = NULL;
  cola -> head_process_ready    = NULL;
  cola -> tail_process_ready    = NULL;
  return cola;
}

void add_process_inactive(Queue* cola, Process* process)
{
  if (!(cola -> head_process_inactive)) // Raul
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

Process* init_process()
{
  Process* process = malloc(sizeof(Process));
  process -> state = 0;
  process -> next = NULL;
  return process;
}