#include "structs.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  // Input argv[1]
  // Output argv[2]

  // Inicializo variables 

  char line[2048];
  char *token;
  int contador_arg;

  // Leo el archivo
  FILE *file = fopen(argv[1], "r");

  fgets(line, sizeof(line), file);            // Leo la primera linea
  int procesos_totales = atoi(line);          // Cuantos Procesos vendrán
  Queue* cola = init_queue();

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
      add_process(cola, process);
      token = strtok(NULL, " ");
      ++contador_arg;
    }
  }

  Print(cola, procesos_totales);

  FreeMem(cola, procesos_totales);

  fclose(file);

  return 0;
}
