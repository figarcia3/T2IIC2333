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

  Process *arreglo_procesos[procesos_totales];

  for (int i = 0; i < procesos_totales; i++)  // Lee cada proceso.
  {
    arreglo_procesos[i] = (Process*)malloc(sizeof(Process));
    arreglo_procesos[i] -> burst_head = NULL;
    arreglo_procesos[i] -> burst_tail = NULL;

    fgets(line, sizeof(line), file);
    token    = strtok(line, " ");
    contador_arg = 0;

    while (token != NULL)                     // Recibo cada parámetro
    {                                         // El contador_arg se encarga de ver en que argumento vamos.
      if (contador_arg == 0)
      {
        strcpy(arreglo_procesos[i]->nombre, token); // Lee el nombre
      }
      else if (contador_arg == 1)
      {
        arreglo_procesos[i]->pid = atoi(token);     // Lee el PID
      }
      else if (contador_arg == 2)
      {
        arreglo_procesos[i]->init_time = atoi(token); // Lee el tiempo total
      }
      else if (contador_arg == 3)
      {
        arreglo_procesos[i]->deadline = atoi(token);  // Lee el deadline
      }
      else if (contador_arg == 4)
      {
        arreglo_procesos[i]->CPU_burst = atoi(token);
      }
      else
      {
        add_burst(arreglo_procesos[i], atoi(token));
      }
      token = strtok(NULL, " ");
      ++contador_arg;
    }
  }

  Print(arreglo_procesos, procesos_totales);

  FreeMem(arreglo_procesos, procesos_totales);

  fclose(file);

  return 0;
}
