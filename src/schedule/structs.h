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

  Process *next;

  int state;

} Process;

typedef struct queue
{
  Process *head_process;
  Process *tail_process;
} Queue;

void     freeList(Burst* head);
void     FreeMem(Queue *cola, int total_process);
void     Print(Queue *cola, int total_process);
void     add_burst(Process *proceso, int time);
Queue*   init_queue(int procesos_totales);
Process* init_process();