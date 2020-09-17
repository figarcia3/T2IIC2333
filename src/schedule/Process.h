#pragma once

typedef struct burst Burst;

struct burst
{
  int type;
  int burst_time;
  Burst* next;
};

typedef struct process Process;

struct process
{
  char nombre[32];
  int pid;
  int init_time;
  int deadline;
  int CPU_burst;

  Burst* burst_head;
  Burst* burst_tail;

  Process* next;

  int state;
  int turnos_CPU;
  int interrupciones;
  int turnaround_time;
  int ready_time;
  int response_time;
  int waiting_time;
  int completed_deadline;
  int response_bool;

};

Process* init_process();
void     add_burst(Process *proceso, int time);
void     destroy_burst(Process* process);
void     destroy_bursts(Burst* current_burst);
void     destroy_process(Process* process);
void     destroy_processes(Process* current_process);
void     output_process(char* name, Process* process);
void     print_process(Process *process);