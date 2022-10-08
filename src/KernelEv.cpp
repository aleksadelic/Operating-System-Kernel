#include "KernelEv.h"
#include "SCHEDULE.H"
#include "System.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	lock
	value = 0;
	blocked = 0;
	this->ivtNo = ivtNo;
	maker = PCB::running;
	IVTEntry::getIVTEntry(ivtNo)->event = this;
	unlock
}

KernelEv::~KernelEv() {
	lock
	blocked = 0;
	IVTEntry::getIVTEntry(ivtNo)->event = 0;
	unlock
}

void KernelEv::wait() {
	lock
	if (PCB::running == maker) {
		if (--value < 0) {
			PCB::running->state = BLOCKED;
			blocked = PCB::running;
			dispatch();
		}
	}
	unlock
}

void KernelEv::signal() {
	lock
	if (++value <= 0) {
		blocked->state = READY;
		Scheduler::put(blocked);
		blocked = 0;
	} else {
		value = 1;
	}
	unlock
}
