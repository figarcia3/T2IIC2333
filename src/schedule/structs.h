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

  struct process *next;

  int state;

} Process;

typedef struct queue
{
  Process *head_process_inactive;
  Process *tail_process_inactive;
  Process *head_process_ready;
  Process *tail_process_ready;
} Queue;

void     freeList(Burst* head);
void     FreeMem(Queue *cola, int total_process);
void     Print(Queue *cola);
void     add_burst(Process *proceso, int time);
Queue*   init_queue();
Process* init_process();
void     add_process_inactive(Queue* cola, Process* process);