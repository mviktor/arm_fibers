#ifndef _FIBERS_H_
#define _FIBERS_H_

typedef void (*fiberfunc)(void *arg);

void yield();
void create_fiber(fiberfunc fn, void *arg, void *stack, unsigned int stack_size);
void start_fibers();

#endif 
