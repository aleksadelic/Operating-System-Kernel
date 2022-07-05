#include "KerSem.h"
#include "System.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "KSemList.h"

extern KSemList allSem;

KernelSem::KernelSem(int init) {
	lock
	value = init;
	blocked = new PCBList();
	timeBlocked = new TimePCBList();
	allSem.add(this);
	unlock
}

KernelSem::~KernelSem() {
	lock
	delete blocked;
	delete timeBlocked;
	allSem.remove(this);
	unlock
}

int KernelSem::wait(Time maxTimeToWait) {
	lock
	if (--value < 0) {
		block(maxTimeToWait);
	} else {
		unlock
		return 1;
	}
	unlock
	return PCB::running->waitRet;
}

void KernelSem::signal() {
	lock
	if (++value <= 0) {
		unblock();
	}
	unlock
}

int KernelSem::val() const {
	return value;
}

void KernelSem::block(Time maxTimeToWait) {
	PCB::running->state = BLOCKED;
	if (maxTimeToWait == 0)
		blocked->add(PCB::running);
	else
		timeBlocked->add(PCB::running, maxTimeToWait);
	dispatch();
}

void KernelSem::unblock() {
	PCB* pcb = blocked->getFirst();
	if (!pcb)
		pcb = timeBlocked->getFirst();
	if (!pcb)
		return;
	pcb->state = READY;
	pcb->waitRet = 1;
	Scheduler::put(pcb);
}

void KernelSem::updateWaitTimes() {
	lock
	for (KSemList::Node* curr = allSem.head; curr != 0; curr = curr->next) {
		int cnt = curr->ksem->timeBlocked->updateWaitList();
		curr->ksem->value += cnt;
	}
	unlock
}
