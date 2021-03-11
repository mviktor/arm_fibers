# Baremetal ARM Fibers

Very small and simple implementation of fibers for ARM. It doesn't require to have an operating system running, i.e. it is osfree (os-less implementation) of fibers.
Currently it doesn't support context switching, i.e. the fibers created can run in single context only, which means if your fiber inside is doing a context switching then the behavior is unpredictable.

Fiber is a lightweight thread of execution. Unlike threads which are scheduled by a kernel, fibers use cooperative mutitasking, i.e. each fiber explicitly gives a control to the next fiber.

## How to use arm fibers

Simple example

``` 
#include <stdio.h>
#include <stdlib.h>
#include "fibers.h"

void readInput(void *arg)
{
  while(1)
  {
    for (int i = 0; i < 100; i++)
    {
      readInput((device)arg, i);
      yield();
    }
  }
}

void updateProgress(void *arg)
{
  while(1)
  {
    updateProgressbar((display)arg);
    yield();
  }
}

int main(int argc, char *argv[])
{
  create_fiber(readInput, (void*)keyboard, (void*)0x30000, 0x1000);
  create_fiber(updateProgress, (void*)display, (void*)0x31000, 0x1000);
  start_fibers();
  
  // this point should never be reached
  return 0;
}

```
Each fiber is created by calling create_fiber function which accepts:
 - pointer to the fiber main function
 - argument that the fiber main function will be called with
 - stack base - you have to make sure this memory is read-write, you can use malloc to allocate that memory
 - stack size

To start the fibers, just call start_fibers() function

Each fiber is giving up a control to next fiber in the queue by calling yield(). In our example above, the execution will jump back and forth between readInputs and updateProgress - and they will execute seemingly in parallel.
