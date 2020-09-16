#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CPU.h"

CPU* init_cpu(int max_process)
{
    CPU* cpu = malloc(sizeof(CPU));
    cpu -> max_process   = max_process;
    cpu -> count_process = 0;
    cpu -> head_process  = NULL;
    cpu -> tail_process  = NULL;
    return cpu;
}

Process* add_process_cpu(CPU* cpu, Process* process)
{
    if (!(cpu -> head_process))
    {
        cpu -> head_process = process;
        cpu -> tail_process = process;
        cpu -> max_deadline = process -> deadline;
        cpu -> count_process ++;
        return NULL;
    }

    if (cpu -> count_process < cpu -> max_process)
    {
        cpu -> count_process ++;
        if (process -> deadline == cpu -> head_process -> deadline)
            {
                if (process -> pid > cpu -> head_process -> pid)
                {
                process -> next     = cpu -> head_process;
                cpu -> head_process = process;
                return NULL;
                }
            }

        else if (process -> deadline > cpu -> head_process -> deadline)
            {
                process -> next     = cpu -> head_process;
                cpu -> head_process = process;
                return NULL;
            }

        if (process -> deadline == cpu -> tail_process -> deadline)
            {
                if (process -> pid < cpu -> tail_process -> pid)
                {
                cpu -> tail_process -> next = process;
                cpu -> tail_process         = process;
                return NULL; 
                }
            }

        else if (process -> deadline < cpu -> tail_process -> deadline)
            {
                cpu -> tail_process -> next = process;
                cpu -> tail_process         = process;
                return NULL;
            }
        Process* last_process    = cpu -> head_process;
        Process* current_process = last_process -> next;
        while (current_process)
        {
            if (process -> deadline == current_process -> deadline)
            {
                if (process -> pid > current_process -> pid)
                {
                    last_process -> next = process;
                    process -> next      = current_process;
                    return NULL;
                }
            }

            else if (process -> deadline > current_process -> deadline)
            {
                last_process -> next = process;
                process -> next      = current_process;
                return NULL;
            }

            last_process    = current_process;
            current_process = current_process -> next;
        }
        return NULL;

    }
    else
    {
        Process* process_return;
        if (process -> deadline == cpu -> head_process -> deadline)
        {
            if (process -> pid < cpu -> head_process -> pid)
            {
                process_return         = cpu -> head_process;
                cpu -> head_process    = process_return -> next;
                process_return -> next = NULL;
                cpu -> count_process --;
                add_process_cpu(cpu, process);
                return process_return;
            }
        }
        
        else if (process -> deadline < cpu -> head_process -> deadline)
        {
            process_return         = cpu -> head_process;
            cpu -> head_process    = process_return -> next;
            process_return -> next = NULL;
            cpu -> count_process --;
            add_process_cpu(cpu, process);
            return process_return;
        }
        return process;
    } 
}

void destroy_cpu(CPU* cpu)
{
    destroy_processes(cpu -> head_process);
    free(cpu);
}