#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "Queue.h"
#include "Process.h"
#include "CPU.h"

int main(int argc, char *argv[])
{
  // Input argv[1]
  // Output argv[2]

  // Inicializo variables 

  char line[2048];
  char *token;
  int contador_arg;
  int count_cpu;

  if (argv[3])
  {
    count_cpu = atoi(argv[3]);
  }
  else
  {
    count_cpu = 1;
  }

  // Leo el archivo
  FILE *file = fopen(argv[1], "r");

  fgets(line, sizeof(line), file);            // Leo la primera linea
  int procesos_totales = atoi(line);          // Cuantos Procesos vendrán

  CPU* cpu = init_cpu(count_cpu);
  Queue* cola = init_queue(procesos_totales);

  for (int i = 0; i < procesos_totales; i++)  // Lee cada proceso.
  {
    Process* process = init_process();

    fgets(line, sizeof(line), file);
    token    = strtok(line, " ");
    contador_arg = 0;

    while (token != NULL)                     // Recibo cada parámetro
    {                                         // El contador_arg se encarga de ver en que argumento vamos.
      if (contador_arg == 0)
      {
        strcpy(process->nombre, token); // Lee el nombre
      }
      else if (contador_arg == 1)
      {
        process->pid = atoi(token);     // Lee el PID
      }
      else if (contador_arg == 2)
      {
        process->init_time = atoi(token); // Lee el tiempo total
      }
      else if (contador_arg == 3)
      {
        process->deadline = atoi(token);  // Lee el deadline
      }
      else if (contador_arg == 4)
      {
        process->CPU_burst = atoi(token);
      }
      else
      {
        add_burst(process, atoi(token));
      }
      token = strtok(NULL, " ");
      ++contador_arg;
    }
    add_process_inactive(cola, process);
  }

  fclose(file);

  int time = 0;
  while (true)
  {
    update_cpu(cola, cpu, time); // ejecutamos CPU (cpu -> waiting o finished)

    update_inactive(cola, time);        // inactive -> ready
    update_ready(cola, cpu, time);      // ready -> cpu
    update_waiting(cola);               // wating -> ready

    if (!(cola -> pending_processes))
    {
      break;
    }

    time ++;
  }

  output_process(argv[2], cola->head_process_finished);
  destroy_queue(cola);
  destroy_cpu(cpu);

  return 0;
}
