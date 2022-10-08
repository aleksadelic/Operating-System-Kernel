#include "PCB.h"
#include <dos.h>
#include "System.h"

#define nullptr 0

ID PCB::lastID = 0;
PCB* PCB::running = nullptr;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	if (stackSize > 65536) stackSize = 65536;
	stackSize /= sizeof(unsigned);
	stack = new unsigned[stackSize];
	quantum = timeSlice;
	id = ++lastID;
	myThread = t;
	state = NEW;
	waiting = new PCBList();
	waitRet = 0;

	stack[stackSize - 1] = 0x200; // PSWI = 1
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(PCB::wrapper);
	stack[stackSize - 3] = FP_OFF(PCB::wrapper);

	sp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);
	bp = sp;
#endif

}

PCB::~PCB() {
	delete[] stack;
	delete waiting;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	lock
	PCB::running->waiting->wakeUp();
	PCB::running->state = FINISHED;
	unlock
	dispatch();
}
