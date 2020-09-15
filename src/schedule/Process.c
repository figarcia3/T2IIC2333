#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"

Process* init_process()
{
  Process* process      = malloc(sizeof(Process));
  process -> state      = 0;
  process -> next       = NULL;
  process -> burst_head = NULL;
  process -> burst_tail = NULL;
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