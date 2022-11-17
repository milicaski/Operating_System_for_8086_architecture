#include "ListSig.h"


ListSig::ListSig(){
	head = tail = 0;
	numSignals = 0;
	front = 0;
}

ListSig::~ListSig(){
	front = 0;
	while (head != 0){
		remove();
	}
}

void ListSig::put(int newSignal){
	Node *newNode = new Node(newSignal);
	if (head == 0){
		head = newNode;
	}else{
		tail->next= newNode;
		newNode->prev = tail;
	}
	tail = newNode;

	numSignals++;
}

int ListSig::get(){
	int ret = -1;
	if(head != 0){
		if(front == 0){
			front = head;
		}
		ret = front->signal;
	}
	return ret;
}

int ListSig::remove(){
	int ret = -1;
	if(head != 0){
		Node* old = 0;
		if (front == 0){
			old = head;
			ret = head->signal;
			head = head->next;
			if (head)
				head->prev = 0;
			if (head == 0)
				head = tail = 0;
		}else{
			old = front;
			ret = front->signal;
			front = front->next;

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

		}
		delete old;
		numSignals--;
	}
	return ret;
}

void ListSig::skip(){
	if(front != 0){
		front = front->next;
	}
}

int ListSig::getNumSignals(){
	return numSignals;
}
