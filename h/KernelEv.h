#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"
#include "PCB.h"

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();

private:
	PCB* blocked;
	int value;
	IVTNo ivtNo;
	PCB* maker;
};

#endif /* KERNELEV_H_ */
