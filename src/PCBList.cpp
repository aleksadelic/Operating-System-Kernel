#include "PCBList.h"
#include "PCB.h"
#include "SCHEDULE.H"

PCBList::PCBList() {
	head = tail = nullptr;
}

PCBList::~PCBList() {
	while (head) {
		Node* old = head;
		head = head->next;
		delete old;
	}
	tail = nullptr;
}

void PCBList::add(PCB* pcb) {
	Node* node = new Node(pcb);
	if (!head)
		head = tail = node;
	else
		tail = tail->next = node;
}

void PCBList::remove(PCB* pcb) {
	for (Node* curr = head, *prev = nullptr; curr != nullptr;) {
		if (curr->pcb == pcb) {
			Node* old = curr;
			curr = curr->next;
			if (!prev)
				head = curr;
			else
				prev->next = curr;

			if (old == tail)
				tail = prev;
			delete old;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}
}

PCB* PCBList::getPCBById(int id) {
	for (Node* curr = head; curr != nullptr; curr = curr->next) {
		if (curr->pcb->id == id)
			return curr->pcb;
	}
	return nullptr;
}

void PCBList::wakeUp() {
	while (head) {
		Node* old = head;
		head->pcb->state = READY;
		Scheduler::put(head->pcb);
		head = head->next;
		delete old;
	}
	tail = nullptr;
}

PCB* PCBList::getFirst() {
	if (!head)
		return 0;
	Node* first = head;
	PCB* firstPCB = first->pcb;
	head = head->next;
	delete first;
	return firstPCB;
}
