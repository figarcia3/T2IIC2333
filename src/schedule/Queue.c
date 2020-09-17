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

void add_process_inactive(Queue* cola, Process* process) // Estamos Listos.
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
  if (!(cola -> head_process_ready)) // Revisamos si existe la cabeza.
  {
    cola -> head_process_ready = process;
    cola -> tail_process_ready = process;
    return;
  }

  if (process -> deadline == cola -> head_process_ready -> deadline) // Comparamos con el deadline del primero.
  {
    if (process -> pid < cola -> head_process_ready -> pid) // Si es igual el deadline, decide el pid.
    {
      process -> next            = cola -> head_process_ready;
      cola -> head_process_ready = process;
      return;
    }
  }

  else if (process -> deadline < cola -> head_process_ready -> deadline) // Comparamos con el deadline del primero.
  {
    process -> next            = cola -> head_process_ready;
    cola -> head_process_ready = process;
    return;
  }

  if (process -> deadline == cola -> tail_process_ready -> deadline) // Comparamos con el deadline del ultimo.
  {
    if (process -> pid > cola -> tail_process_ready -> pid) // Si es igual el deadline. Decide el Pid.
    {
      cola -> tail_process_ready -> next = process;
      cola -> tail_process_ready         = process;
      return; 
    }
  }

  else if (process -> deadline > cola -> tail_process_ready -> deadline) // Comparamos con el deadline del ultimo.
  {
    cola -> tail_process_ready -> next = process;
    cola -> tail_process_ready         = process;
    return;
  }

  Process* last_process    = cola -> head_process_ready;
  Process* current_process = last_process -> next;
  while (current_process) // El proceso lo ubicamos en su lugar correcto.
  {
    if (process -> deadline == current_process -> deadline) // Comparamos
    {
      if (process -> pid < current_process -> pid) // Si son iguales, entramos con PID.
      {
        last_process -> next = process;
        process -> next      = current_process;
        return;
      }
    }

    else if (process -> deadline < current_process -> deadline) // Comparamos.
    {
      last_process -> next = process;
      process -> next      = current_process;
      return;
    }

    last_process    = current_process;
    current_process = current_process -> next;
  }
}

void add_process_waiting(Queue* cola, Process* process) // Agregamos al final de la lista.
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

void add_process_finished(Queue* cola, Process* process) // Agregamos al final de la lista.
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
    if (current_process->init_time == time) // Si el time es igual al tiempo de inicio. Agregamos el proceso.
    {
      next_process                = current_process->next;

      current_process->next       = NULL;
      current_process->state      = 1;
      current_process->ready_time = time;

      add_process_ready(cola, current_process);

      current_process             = next_process;
      cola->head_process_inactive = current_process;

      continue;
    }
    break;
  }
}

void update_ready(Queue* cola, CPU* cpu, int time)
{
    Process* break_process;
    Process* current_process = cola -> head_process_ready;
    while(current_process)
    {
      if (current_process == cola -> tail_process_ready)
      {
        cola -> tail_process_ready = NULL;
      }
      cola -> head_process_ready = current_process -> next;
      current_process -> next    = NULL;
      break_process = add_process_cpu(cpu, current_process);
      if (break_process == current_process)
      {
        add_process_ready(cola, break_process);
        break;
      }
      else if (break_process)
      {
        break_process -> interrupciones++;
        add_process_ready(cola, break_process);
      }

      if (!current_process -> response_bool)
      {
        current_process -> response_bool = 1;
        current_process -> response_time = time - current_process -> ready_time;
      }
      current_process = cola -> head_process_ready;
    }
    current_process = cola -> head_process_ready;
    while(current_process)
    {
      current_process -> waiting_time++;
      current_process = current_process -> next;
    }
}

void update_waiting(Queue* cola)
{
  Process* last_process = NULL;
  Process* current_process = cola -> head_process_waiting;
  while (current_process)
  {
    current_process -> waiting_time++;
    current_process -> burst_head -> burst_time--;
    if (current_process -> burst_head -> burst_time == 0)       // Un IO Burst llega a 0. Debemos mover el proceso a ready.
    {
      destroy_burst(current_process);
      if (current_process == cola -> head_process_waiting)      // El proceso está al inicio.
      {
        if (current_process == cola -> tail_process_waiting)    // Solo había un proceso.
        {
          current_process -> next = NULL; // Verificamos que no haya nada.
          add_process_ready(cola, current_process);
          cola -> head_process_waiting = NULL;  // Setiamos la cabeza.
          cola -> tail_process_waiting = NULL;  // Setiamos la cola.
          break;
        }
        else                                                    // Habían mas procesos..
        {
          cola -> head_process_waiting = current_process -> next; // Setieamos la cabeza.
          current_process -> next      = NULL; // Desconecto el currrent_process
          add_process_ready(cola, current_process);
          current_process = cola -> head_process_waiting; // Seguimos la ejecucion.
          continue;
        }
      }
      else if (current_process == cola -> tail_process_waiting) // El proceso está al final. Al llegar al final, debemos salir.
      {
        last_process -> next         = NULL; // Desconectamos al current_process por detras.
        cola -> tail_process_waiting = last_process; // Setiamos la nueva cola.
        current_process -> next      = NULL; // Verificamos que no esté conectado por delante.
        add_process_ready(cola, current_process);
        break;
      }
      else // El proceso que entra está al medio de 2 procesos.
      {
        last_process -> next    = current_process -> next; // Desconectamos por la izquierda y unimos los 2 que quedan.
        current_process -> next = NULL; // Deconectamos por la derecha.
        add_process_ready(cola, current_process); 
        current_process = last_process -> next;
        continue;
      }
    }
    last_process    = current_process;
    current_process = current_process -> next;
  }
}

void extract_process_cpu(Queue* cola, Process* process, int time)
{
    if (!process -> burst_head) // Entra si termina ya que no quedan burst restantes de ningun tipo.
    {
      process -> turnaround_time = time - process -> ready_time; // obtenemos
      if (process -> deadline >= time) // Entra si termina en dentor de su deadline.
      {
        process -> completed_deadline = 1;
      }
      else
      {
        process -> completed_deadline = 0;
      }
      add_process_finished(cola, process);
    }
    else
    {
      add_process_waiting(cola, process);
    }
}

void update_cpu(Queue* cola, CPU* cpu, int time)
{
    Process* last_process    = NULL;
    Process* current_process = cpu -> head_process;

    while (current_process)
    {
        current_process -> burst_head -> burst_time--; // Restamos en el burst correspondiente.
        if (current_process -> burst_head -> burst_time == 0)
        {
            destroy_burst(current_process); // Sale el burst.
            cpu -> count_process--; // Descontamos del contador de procesador.
            if ((!last_process) && (current_process == cpu -> tail_process)) // Hay solo uno en la cola
            {
                current_process -> next = NULL;
                extract_process_cpu(cola, current_process, time);
                cpu -> head_process = NULL;
                cpu -> tail_process = NULL;
                break;
            }

            else if (!last_process) // Si es cabeza no mas.
            {
                cpu -> head_process      = current_process -> next; // Setiamos la cabeza.
                current_process -> next  = NULL;
                extract_process_cpu(cola, current_process, time);
                current_process = cpu -> head_process;
                continue;
            }

            if (current_process == cpu -> tail_process)
            {
                last_process -> next    = NULL;
                cpu -> tail_process     = last_process;
                current_process -> next = NULL;
                extract_process_cpu(cola, current_process, time);
                break;
            }
            
            last_process -> next    = current_process -> next;  // unir anterior con el siguiente
            current_process -> next = NULL; 
            extract_process_cpu(cola, current_process, time);
            current_process = last_process -> next; // siguiente como current process
            continue;
        }

        last_process    = current_process;
        current_process = current_process -> next;
    }
}

void destroy_queue(Queue* cola)
{
  destroy_processes(cola -> head_process_inactive);
  destroy_processes(cola -> head_process_ready);
  destroy_processes(cola -> head_process_finished);
  free(cola);
}