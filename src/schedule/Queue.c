#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

Queue* init_queue(int pending_process)
{
  Queue* cola = malloc(sizeof(Queue));
  cola -> pending_processes      = pending_process;
  cola -> head_process_inactive  = NULL;
  cola -> tail_process_inactive  = NULL;
  cola -> head_process_ready     = NULL;
  cola -> tail_process_ready     = NULL;
  cola -> head_process_waiting   = NULL;
  cola -> tail_process_waiting   = NULL;
  cola -> head_process_finished  = NULL;
  cola -> tail_process_finished  = NULL;

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

void add_process_waiting(Queue* cola, Process* process)
{
  if (!(cola->head_process_waiting))
  {
    cola -> head_process_waiting = process;
    cola -> tail_process_waiting = process;
  }
  else
  {
    cola -> tail_process_waiting -> next = process;
    cola -> tail_process_waiting = process;
  }
}

void add_process_finished(Queue* cola, Process* process)
{
  cola -> pending_processes--;
  if (!(cola -> head_process_finished))
  {
    cola -> head_process_finished = process;
    cola -> tail_process_finished = process;
  }
  else
  {
    cola -> tail_process_finished -> next = process;
    cola -> tail_process_finished = process; 
  }
}

void update_inactive(Queue* cola, int time)
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

void update_ready(Queue* cola, CPU* cpu)
{
    Process* break_process;
    Process* current_process = cola -> head_process_ready;
    while(current_process)
    {
        cola -> head_process_ready = current_process -> next;
        current_process -> next    = NULL;
        break_process = add_process_cpu(cpu, current_process);
        if (break_process)
        {
            add_process_ready(cola, break_process);
            break;
        }
        current_process = cola -> head_process_ready;
    }
}

void update_waiting(Queue* cola)
{
  Process* last_process = NULL;
  Process* current_process = cola -> head_process_waiting;
  while (current_process)
  {
    current_process -> burst_head -> burst_time--;
    if (current_process -> burst_head -> burst_time == 0)       // Un IO Burst llega a 0. Debemos mover el proceso a ready.
    {
      destroy_burst(current_process);
      if (current_process == cola -> head_process_waiting)      // El proceso está al inicio.
      {
        if (current_process == cola -> tail_process_waiting)
        {
          current_process -> next = NULL;
          add_process_ready(cola, current_process);
          cola -> head_process_waiting = NULL;
          cola -> tail_process_waiting = NULL;
          break;
        }
        else
        {
          cola -> head_process_waiting = current_process -> next;
          current_process -> next      = NULL;
          add_process_ready(cola, current_process);
          current_process = cola -> head_process_waiting;
          continue;
        }
      }
      else if (current_process == cola -> tail_process_waiting) // El proceso está al final. Al llega al final, debemos salir.
      {
        last_process -> next         = NULL;
        cola -> tail_process_waiting = last_process;
        current_process -> next      = NULL;
        add_process_ready(cola, current_process);
        break;
      }
      else // El proceso está al medio de 2 procesos.
      {
        last_process -> next    = current_process -> next;
        current_process -> next = NULL;
        add_process_ready(cola, current_process);
        current_process = last_process -> next;
      }
    }
    last_process    = current_process;
    current_process = current_process -> next;
  }
}

void extract_process_cpu(Queue* cola, Process* process)
{
    if (!process -> burst_head)
    {
        add_process_finished(cola, process);
    }
    else
    {
        add_process_waiting(cola, process);
    }
}

void update_cpu(Queue* cola, CPU* cpu)
{
    Process* last_process    = NULL;
    Process* current_process = cpu -> head_process;

    while (current_process)
    {
        current_process -> burst_head -> burst_time--;
        if (current_process -> burst_head -> burst_time == 0)
        {
            destroy_burst(current_process);
            if ((!last_process) && (current_process == cpu -> tail_process))
            {
                current_process -> next = NULL;
                extract_process_cpu(cola, current_process);
                cpu -> head_process = NULL;
                cpu -> tail_process = NULL;
                break;
            }

            else if (!last_process)
            {
                cpu -> head_process      = current_process -> next;
                current_process -> next  = NULL;
                extract_process_cpu(cola, current_process);
                current_process = cpu -> head_process;
                continue;
            }

            if (current_process == cpu -> tail_process)
            {
                last_process -> next    = NULL;
                cpu -> tail_process     = last_process;
                current_process -> next = NULL;
                extract_process_cpu(cola, current_process);
                break;
            }
            
            last_process -> next    = current_process -> next;
            current_process -> next = NULL;
            extract_process_cpu(cola, current_process);
            current_process = last_process -> next;
        }

        last_process    = current_process;
        current_process = current_process -> next;
    }
}

void destroy_queue(Queue* cola)
{
  destroy_processes(cola -> head_process_inactive);
  destroy_processes(cola -> head_process_ready);
  free(cola);
}