/*
   Copyright 2021 Viktor Mladenovski

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdlib.h>
#include "fibers.h"

enum fiber_status
{
    E_NOTSTARTED=0,
    E_RUNNING,
    E_IDLE,
    E_DEAD
};

typedef struct fiber
{
    char*               name;
    fiberfunc           fn;
    void*               arg;
    void*               stack;
    unsigned int        stack_size;
    enum fiber_status   status;
} fiber;

typedef enum {FALSE=0,TRUE=1} BOOLEAN;

void asm_start_fiber(void *arg, void *stack, fiberfunc fn);
void asm_jump_to_running_fiber(void *stack);

int running_fiber=-1;
int total_fibers=0;
fiber fibers[10];
BOOLEAN fibers_started=FALSE;

void save_and_schedule_next(void *stack)
{
    if (running_fiber >= 0)
    {
        fibers[running_fiber].stack = stack;
        if (fibers[running_fiber].status == E_RUNNING)
            fibers[running_fiber].status = E_IDLE;
    }

    do
    {
        running_fiber++;
        if (running_fiber >= total_fibers)
            running_fiber = 0;

        switch (fibers[running_fiber].status)
        {
        case E_NOTSTARTED:
            fibers[running_fiber].status = E_RUNNING;
            asm_start_fiber(fibers[running_fiber].arg, fibers[running_fiber].stack + fibers[running_fiber].stack_size, fibers[running_fiber].fn);
            break;

        case E_RUNNING:
        case E_IDLE:
            fibers[running_fiber].status = E_RUNNING;
            asm_jump_to_running_fiber(fibers[running_fiber].stack);
            break;
        
        default:
            break;
        }
    } while (1);
}

void fiber_destroy_self()
{
    fibers[running_fiber].status = E_DEAD;
    yield();
}

void create_fiber(fiberfunc fn, void *arg, void *stack, unsigned int stack_size)
{
    fibers[total_fibers].status = E_NOTSTARTED;
    fibers[total_fibers].fn = fn;
    fibers[total_fibers].arg = arg;
    fibers[total_fibers].stack = stack;
    fibers[total_fibers].stack_size = stack_size;
    total_fibers++;
}

void start_fibers()
{
    if (fibers_started == TRUE)
        return;
    fibers_started = TRUE;
    save_and_schedule_next(NULL);
}
