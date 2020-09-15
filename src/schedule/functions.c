#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void freeList(Burst* head)
{
  Burst* tmp;
  while (head)
  {
      tmp = head;
      head = head->next;
      free(tmp);
  }
}

void freeMem(Process *arreglo_process[], int total_process)
{
  for (int i = 0; i < total_process; i++) // Libero memoria arreglo.
  {
    freeList(arreglo_process[i]->burst_head);
    free(arreglo_process[i]);
  }
}

void Print(Process *arreglo_process[], int total_process)
{
  Burst *tmp;

  for (int i = 0; i < total_process; i++) // Verificamos si la informacion se leyó correctamente. 
  {
  printf("Nombre Proceso: %s\n", arreglo_process[i]->nombre);
  printf("Pid: %i\n",            arreglo_process[i]->pid);
  printf("Inicio: %i\n",         arreglo_process[i]->init_time);
  printf("Deadline: %i\n",       arreglo_process[i]->deadline);
  printf("CPU_burst: %i\n",      arreglo_process[i]->CPU_burst);

  tmp = arreglo_process[i] ->burst_head;
  
  while (tmp)
  {
    printf("type: %i, time: %i\n", tmp -> type, tmp -> burst_time);
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