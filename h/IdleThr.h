#ifndef IDLETHR_H_
#define IDLETHR_H_

#include "Thread.h"

class IdleThread: public Thread {
public:
	IdleThread() : Thread(1024, 1) {}
	void run();
};

#endif /* IDLETHR_H_ */
