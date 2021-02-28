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

#include <stdio.h>
#include <stdlib.h>
#include "fibers.h"

int mystrlen(char *str)
{
    int res=0;
    while (*str++)
    {
        res++;
        yield();
    }

    return res;
}

void fn(void *arg)
{
    int res = mystrlen((char*)arg);
    (void)res;
}

int main(int argc, char *argv[])
{
    create_fiber(fn, "String 1", (void*)0x30000, 0x1000);
    create_fiber(fn, "short string", (void*)0x31000, 0x1000);
    create_fiber(fn, "long long long string", (void*)0x32000, 0x1000);
    start_fibers();
    return 0;
}
