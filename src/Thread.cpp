#include "Thread.h"
#include "PCB.h"
#include "PCBList.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include "System.h"

extern PCBList allPCB;

void Thread::start() {
	if (myPCB->state == NEW) {
		myPCB->state = READY;
		Scheduler::put(myPCB);
	}
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	allPCB.add(myPCB);
	unlock
}

Thread::~Thread() {
	this->waitToComplete();
	lock
	allPCB.remove(myPCB);
	delete myPCB;
	unlock
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id) {
	lock
	PCB* pcb = allPCB.getPCBById(id);
	unlock
	if (!pcb) {
		printf("Thread with ID %d does not exist!\n", id);
		return nullptr;
	} else {
		return pcb->myThread;
	}
}

void Thread::waitToComplete() {
	lock
	if (PCB::running == myPCB || myPCB->state == FINISHED || myPCB->state == NEW || myPCB->state == IDLE) {
		unlock
		return;
	}
	myPCB->waiting->add(PCB::running);
	PCB::running->state = BLOCKED;
	unlock
	dispatch();
}

void dispatch() {
	lock
	setContextSwitchOnDemand();
	timer();
	unlock
}
