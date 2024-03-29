#include "types.h"
#include "mmu.h"
#include "param.h"
#include "defs.h"
#include "proc.h"
#include "x86.h"


int gpid() {
	int p = myproc()->parent->parent->pid;
	return p;
}

//Wrapper for my gpid
int sys_gpid(void) {
	int pid;
	if(argint(0, &pid) < 0)
		return -1;
	return gpid();
}
