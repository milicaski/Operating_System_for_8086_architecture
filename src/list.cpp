#include "list.h"
#include <iostream.h>


List::List(){
	head = tail = 0;
	numNodes=0;
	curr = 0;
}

List:: ~List(){
	while (head != 0){
		removeFirst();
	}
}


void List:: add(PCB* newPCB){
	Node *newNode = new Node(newPCB);
	if (head == 0){
		head = newNode;
	}else{
		tail->next= newNode;
		newNode->prev = tail;
	}
	tail = newNode;

	numNodes++;
}

PCB* List::getFirst(){
	PCB* first = 0;
	if (head != 0){
		first = head->pcb;
	}
	return first;
}

PCB* List::removeFirst(){
	PCB* first = 0;
	if (head != 0){
		first = head->pcb;
		Node *old = head;
		head = head->next;
		if (head)
			head->prev = 0;
		if (head == 0)
			head = tail = 0;
		numNodes--;
		delete old;
	}
	return first;
}

void List::currAtFirst(){
	if (head != 0)
		curr = head;
	else
		curr = 0;
}

void List::moveCurr(){
	if (curr != 0)
		curr = curr->next;
}

int List::isCurr(){
	if (curr != 0)
		return 1;
	else
		return 0;
}

PCB* List::getCurr(){
	PCB* ret = 0;
		if (curr != 0){
			ret = curr->pcb;
		}
	return ret;
}


PCB* List::removeCurr(){
	PCB* ret = 0;
	if (curr != 0){
		ret = curr->pcb;
		Node *old = curr;
		curr = curr->next;

		if (old->prev != 0){
			old->prev->next = old->next;
		}else{
			head = old->next;
		}
		if (old->next != 0){
			old->next->prev = old->prev;
		}else{
			tail = old->prev;
		}
		delete old;
		numNodes--;
	}
	return ret;
}

int List:: getNumNodes(){
	return numNodes;
}
