#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "PCBList.h"

enum State { NEW, READY, RUNNING, BLOCKED, FINISHED, IDLE };

class PCB {
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;

	unsigned* stack;
	unsigned quantum;

	State state;

	static int lastID;
	int id;

	int waitRet;

	Thread* myThread;

	static PCB* running;

	PCBList* waiting;

	PCB(Thread* t = 0, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	~PCB();

	static void wrapper();
};

#endif /* PCB_H_ */
