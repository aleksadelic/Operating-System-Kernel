#ifndef KSEMLIST_H_
#define KSEMLIST_H_

#include "KerSem.h"

class KSemList {
private:
	struct Node {
		KernelSem* ksem;
		Node* next;
		Node(KernelSem* k, Node* n = 0) : ksem(k), next(n) {}
	};
	Node* head;
	Node* tail;

	friend class KernelSem;

public:
	KSemList();

	~KSemList();

	void add(KernelSem* ksem);

	void remove(KernelSem* ksem);

};

#endif /* KSEMLIST_H_ */
