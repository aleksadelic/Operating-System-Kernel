#ifndef TPCBLIST_H_
#define TPCBLIST_H_

#define nullptr 0

typedef unsigned int Time;

class PCB;

class TimePCBList {
private:
	struct Node {
		PCB* pcb;
		Node* next;
		Time timeToWait;
		Node(PCB* p, Time t, Node* n = nullptr) : pcb(p), next(n), timeToWait(t) {}
	};
	Node* head;

	friend class KernelSem;

public:
	TimePCBList();

	~TimePCBList();

	void add(PCB* pcb, Time timeToWait);

	PCB* getFirst();

	int updateWaitList();
};

#endif /* TPCBLIST_H_ */
