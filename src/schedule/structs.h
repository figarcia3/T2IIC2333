#define MAX 255

typedef struct burst
{
  int type;
  int burst_time;
  struct burst *next;
} Burst;

typedef struct process
{
  char nombre[32];
  int pid;
  int init_time;
  int deadline;
  int CPU_burst;

  Burst *burst_head;
  Burst *burst_tail;

  int state;
} Process;

typedef struct queu
{
  Process *array;
} Queu;

void freeList(Burst* head);
void freeMem(Process *arreglo_process[], int total_process);
void Print(Process *arreglo_process[], int total_process);
void add_burst(Process *proceso, int time);