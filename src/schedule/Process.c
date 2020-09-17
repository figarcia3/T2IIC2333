#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"

Process* init_process()
{
  Process* process              = malloc(sizeof(Process));
  process -> state              = 0;
  process -> next               = NULL;
  process -> burst_head         = NULL;
  process -> burst_tail         = NULL;
  process -> response_bool      = 0;
  process -> response_time      = 0;
  process -> turnos_CPU         = 0;
  process -> interrupciones     = 0; 
  process -> turnaround_time    = 0;
  process -> ready_time         = 0;
  process -> response_time      = 0;
  process -> waiting_time       = 0;
  process -> completed_deadline = 0;
  return process;
}

void add_burst(Process *proceso, int time) // Agrega un Burst al final de la cola.
{
  Burst* burst = malloc(sizeof(Burst));
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

void destroy_burst(Process* process)
{
  Burst* delete_burst   = process -> burst_head;
  process -> burst_head = process -> burst_head -> next;
  free(delete_burst);
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

void destroy_process(Process* process)
{
  free(process);
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

void output_process(char* name, Process* process)
{
  FILE* file = fopen(name, "w+");
  
  Process* current = process;
  while(current)
  {
    fprintf(file, "%s,%i,%i,%i,%i,%i,%i\n", current->nombre,current->turnos_CPU,current->interrupciones,
                                            current->turnaround_time,current->response_time,current->waiting_time,
                                            current->completed_deadline);
    current = current -> next;
  }
  fclose(file);
}

void print_process(Process *process)
{
  Burst* tmp;
  Process* tmp_process;

  tmp_process = process;

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