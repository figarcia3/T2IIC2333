#pragma once

#include "Process.h"

typedef struct cpu CPU;

struct cpu
{
    int max_deadline;
    int max_process;
    int count_process;

    Process* head_process;
    Process* tail_process;
  
};

CPU*     init_cpu(int max_process);
Process* add_process_cpu(CPU* cpu, Process* process);
void     destroy_cpu(CPU* cpu); 