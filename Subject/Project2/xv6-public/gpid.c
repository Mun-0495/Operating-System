#include "types.h"
#include "mmu.h"
#include "defs.h"
#include "param.h"
#include "proc.h"
#include "x86.h"

int gpid() {
	int p = myproc()->parent->parent->pid;
	return p;
}

//Wrapper for my gpid
int sys_gpid(void) {
	int pid = myproc()->pid;
	if(pid < 0)
		return -1;
	return gpid();
}
