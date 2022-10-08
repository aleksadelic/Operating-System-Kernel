#ifndef KERSEM_H_
#define KERSEM_H_

#include "PCBList.h"
#include "Semaphor.h"
#include "TPCBList.h"

class KernelSem {
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	int val() const; // Returns the current value of the semaphore

	static void updateWaitTimes();

protected:
	void block(Time maxTimeToWait);
	void unblock();

private:
	PCBList* blocked;
	TimePCBList* timeBlocked;
	int value;
};

#endif /* KERSEM_H_ */
