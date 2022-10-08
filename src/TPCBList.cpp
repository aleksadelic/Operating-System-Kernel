#include "TPCBList.h"
#include "PCB.h"
#include "SCHEDULE.H"

TimePCBList::TimePCBList() {
	head = nullptr;
}

TimePCBList::~TimePCBList() {
	while (head) {
		Node* old = head;
		head = head->next;
		delete old;
	}
}

void TimePCBList::add(PCB* pcb, Time timeToWait) {
	Node* node = new Node(pcb, timeToWait);
	if (!head) {
		head = node;
		return;
	}
	Node* curr = head, *prev = nullptr;
	while (curr != nullptr && timeToWait > curr->timeToWait) {
		timeToWait -= curr->timeToWait;
		prev = curr;
		curr = curr->next;
	}
	if (prev)
		prev->next = node;
	node->next = curr;
	node->timeToWait = timeToWait;
	if (curr)
		curr->timeToWait -= timeToWait;
	if (curr == head)
		head = node;
}

PCB* TimePCBList::getFirst() {
	if (!head)
		return 0;
	Node* first = head;
	PCB* firstPCB = first->pcb;
	head = head->next;
	if (head)
		head->timeToWait += first->timeToWait;
	delete first;
	return firstPCB;
}

int TimePCBList::updateWaitList() {
	if (!head)
		return 0;
	int cntRemoved = 0;
	head->timeToWait--;
	while (head != nullptr && head->timeToWait == 0) {
		Node* old = head;
		PCB* pcb = head->pcb;
		pcb->state = READY;
		pcb->waitRet = 0;
		Scheduler::put(pcb);
		head = head->next;
		delete old;
		cntRemoved++;
	}
	return cntRemoved;
}
