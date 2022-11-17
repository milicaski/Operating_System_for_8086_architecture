#include "ListSem.h"
#include <iostream.h>



ListSem::ListSem() {
	head = tail = 0;
	numNodes = 0;
	curr = 0;
}

ListSem::~ListSem() {
	while (head != 0) {
		removeFirst();
	}
}


void ListSem::add(PCB* newPCB, int waitingTime) {
	Node *newNode = new Node(newPCB, waitingTime);

	if (waitingTime == 0) {
		if (head == 0) {
			head = newNode;
		} else {
			tail->next = newNode;
			newNode->prev = tail;
		}
		tail = newNode;
	} else {
		int dif = waitingTime;
		Node *tmp = head;
		int con = 0;
		while (tmp && con + tmp->waitingTime <= waitingTime && tmp->startWaitingTime != 0) {
			con += tmp->waitingTime;
			dif -= tmp->waitingTime;
			tmp = tmp->next;
		}
		newNode->waitingTime = dif;

		if (tmp) { //ispred tmpa treba da se ubaci
			newNode->next = tmp;
			newNode->prev = tmp->prev;
			if (tmp->prev != 0) {
				tmp->prev->next = newNode;
			} else {
				head = newNode;
			}
			tmp->prev = newNode;
			if (tmp->startWaitingTime != 0) {
				tmp->waitingTime = tmp->waitingTime - newNode->waitingTime;
			}

		} else { // ili je prazna lista ili je na kraj treba da se ubaci
			if (head == 0) {
				head = newNode;
			} else {
				tail->next = newNode;
				newNode->prev = tail;
			}
			tail = newNode;
		}
	}
	numNodes++;
}

PCB* ListSem::getFirst() {
	PCB* first = 0;
	if (head != 0) {
		first = head->pcb;
	}
	return first;
}

ListSem::Node* ListSem::getFirstNode() {
	Node* first = 0;
	if (head != 0) {
		first = head;
	}
	return first;
}

PCB* ListSem::removeFirst() {
	PCB* first = 0;
	if (head != 0) {
		first = head->pcb;
		Node *old = head;
		head = head->next;
		if (head){
			head->prev = 0;
			if (head->startWaitingTime != 0){
				head->waitingTime = head->waitingTime + old->waitingTime;
			}
		}
		if (head == 0)
			head = tail = 0;
		numNodes--;
		delete old;
	}

	return first;
}

void ListSem::currAtFirst() {
	if (head != 0)
		curr = head;
	else
		curr = 0;
}

void ListSem::moveCurr() {
	if (curr != 0)
		curr = curr->next;
}

int ListSem::isCurr() {
	if (curr != 0)
		return 1;
	else
		return 0;
}

PCB* ListSem::getCurr() {
	PCB* ret = 0;
	if (curr != 0) {
		ret = curr->pcb;
	}
	return ret;
}

ListSem::Node* ListSem::getCurrNode() {
	Node* ret = 0;
	if (curr != 0) {
		ret = curr;
	}
	return ret;
}

PCB* ListSem::removeCurr() {
	PCB* ret = 0;
	if (curr != 0) {
		ret = curr->pcb;
		Node *old = curr;
		curr = curr->next;

		if (old->prev != 0) {
			old->prev->next = old->next;
		} else {
			head = old->next;
		}
		if (old->next != 0) {
			old->next->prev = old->prev;
		} else {
			tail = old->prev;
		}
		delete old;
		numNodes--;
	}
	return ret;
}

int ListSem::isHead(){
	if (head != 0)
			return 1;
		else
			return 0;
}
int ListSem::getNumNodes() {
	return numNodes;
}
