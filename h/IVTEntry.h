#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"
#include "System.h"

typedef void interrupt (*pIntr)(...);

#define PREPAREENTRY(numEntry, callOldFlag)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if (callOldFlag == 1)\
		newEntry##numEntry.callOldRoutine();\
}

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pIntr newRoutine);
	~IVTEntry();

	void callOldRoutine();
	void signal();

	static IVTEntry* getIVTEntry(IVTNo ivtNo);

	static IVTEntry* IVTEntries[256];

private:
	pIntr oldRoutine;
	KernelEv* event;
	IVTNo ivtNo;

	friend class KernelEv;
};

#endif /* IVTENTRY_H_ */
