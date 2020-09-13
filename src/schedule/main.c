

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct proceso
{
  char nombre[255];
  int pid;
  int tiempo_inicio;
  int deadline;
  int CPU_burst;
  int IO_burst;
} Proceso;

typedef struct cola
{

} Cola;



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

  Proceso *arreglo_procesos[procesos_totales];

  for (int i = 0; i < procesos_totales; i++)  // Lee cada proceso.
  {
    arreglo_procesos[i] = (Proceso*)malloc(sizeof(Proceso));

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
        arreglo_procesos[i]->tiempo_inicio = atoi(token); // Lee el tiempo total
      }
      else if (contador_arg == 3)
      {
        arreglo_procesos[i]->deadline = atoi(token);  // Lee el deadline
      }
      else if (contador_arg == 4)
      {
        arreglo_procesos[i]->CPU_burst = atoi(token);
        arreglo_procesos[i]->IO_burst  = atoi(token) - 1; 
      }
      else
      {
        /* Aqui va la estructura que le daremos a los diferentes CPU_burst y IO_burst */
      }
      

      token = strtok(NULL, " ");
      ++contador_arg;
    }
    
  }

  for (int i = 0; i < procesos_totales; i++) // Verificamos si la informacion se leyó correctamente.
  {
    printf("Nombre Proceso: %s\n", arreglo_procesos[i]->nombre);
    printf("Pid: %i\n",            arreglo_procesos[i]->pid);
    printf("Inicio: %i\n",         arreglo_procesos[i]->tiempo_inicio);
    printf("Deadline: %i\n",       arreglo_procesos[i]->deadline);
    printf("CPU_burst: %i\n",      arreglo_procesos[i]->CPU_burst);
    printf("IO_burst: %i\n",      arreglo_procesos[i]->IO_burst);
  }

  for (int i = 0; i < procesos_totales; i++) // Libero memoria arreglo.
  {
    free(arreglo_procesos[i]);
  }
  
  fclose(file);

  return 0;
}
