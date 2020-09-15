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

void FreeMem(Queue *cola, int total_process)
{
  for (int i = 0; i < total_process; i++) // Libero memoria arreglo.
  {
    FreeList(cola->array[i]->burst_head); // liberando burst
    free(cola->array[i]);// liberando Proceso
  }
  free(cola -> array); // liberando Arreglo
  free(cola); // liberando cola
}

void Print(Queue *cola, int total_process)
{
  Burst *tmp;

  for (int i = 0; i < total_process; i++) // Verificamos si la informacion se leyó correctamente. 
  {
  printf("Nombre Proceso: %s\n", cola->array[i]->nombre);
  printf("Pid: %i\n",            cola->array[i]->pid);
  printf("Inicio: %i\n",         cola->array[i]->init_time);
  printf("Deadline: %i\n",       cola->array[i]->deadline);
  printf("CPU_burst: %i\n",      cola->array[i]->CPU_burst);

  tmp = cola->array[i] -> burst_head;
  
    while (tmp)
    {
      printf("burst: %i\n", tmp -> burst_time);
      tmp = tmp -> next;
    }
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

Queue* init_queue(int procesos_totales)
{
  Queue* cola = malloc(sizeof(Queue));
  cola -> head_process = NULL;
  cola -> tail_process = NULL;
  return cola;
}

Process* init_process()
{
  Process* process = malloc(sizeof(Process));
  process -> state = 0;
  return process;
}