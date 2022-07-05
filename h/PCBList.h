#ifndef PCBLIST_H_
#define PCBLIST_H_

#define nullptr 0

class PCB;

class PCBList {
private:
	struct Node {
		PCB* pcb;
		Node* next;
		Node(PCB* p, Node* n = nullptr) : pcb(p), next(n) {}
	};
	Node* head;
	Node* tail;

public:
	PCBList();

	~PCBList();

	void add(PCB* pcb);

	void remove(PCB* pcb);

	PCB* getFirst();

	PCB* getPCBById(int id);

	void wakeUp();

};

#endif /* PCBLIST_H_ */
