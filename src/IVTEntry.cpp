#include "IVTEntry.h"
#include <DOS.H>

IVTEntry* IVTEntry::IVTEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pIntr newRoutine) {
	lock
#ifndef  BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif
	IVTEntries[ivtNo] = this;
	event = 0;
	this->ivtNo = ivtNo;
	unlock
}

IVTEntry::~IVTEntry() {
	lock
	oldRoutine();
#ifndef  BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
	IVTEntries[ivtNo] = 0;
	unlock
}

void IVTEntry::callOldRoutine() {
	oldRoutine();
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo ivtNo) {
	return IVTEntries[ivtNo];
}

void IVTEntry::signal() {
	event->signal();
}
