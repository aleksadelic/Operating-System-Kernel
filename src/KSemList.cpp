#include "KSemList.h"

KSemList::KSemList() {
	head = tail = nullptr;
}

KSemList::~KSemList() {
	while (head) {
		Node* old = head;
		head = head->next;
		delete old;
	}
	tail = nullptr;
}

void KSemList::add(KernelSem* ksem) {
	if (!head)
		head = tail = new Node(ksem);
	else
		tail = tail->next = new Node(ksem);
}

void KSemList::remove(KernelSem* ksem) {
	for (Node* curr = head, *prev = nullptr; curr != nullptr;) {
		if (curr->ksem == ksem) {
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
