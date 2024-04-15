#include "types.h"
#include "defs.h"
#include "param.h"
#include "proc.h"

void sys_yield(void) {
    if(myproc()->state == RUNNING) 
        yield();
    return;
}