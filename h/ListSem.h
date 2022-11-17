#ifndef _LISTSEM_H_
#define _LISTSEM_H_

#include "PCB.h"
class KernelSem;

class ListSem{
public:
	struct Node{
		PCB* pcb;
		int startWaitingTime;
		int waitingTime;
		Node *next;
		Node *prev;
		Node(PCB* newPCB, int swt){
			pcb = newPCB;
			startWaitingTime = swt;
			waitingTime = swt;
			next = 0;
			prev = 0;
		}
		~Node(){
			pcb = 0;
			next = 0;
			prev = 0;
		}
	};
	ListSem();
	~ListSem();
	void add(PCB *newPCB, int waitingTime);
	PCB* getFirst();
	Node* getFirstNode();
	int isHead();
	PCB* removeFirst();
	void currAtFirst();
	void moveCurr();
	int isCurr();
	PCB* getCurr();
	Node* ListSem::getCurrNode();
	PCB* removeCurr();
	int getNumNodes();
private:
	Node *head;
	Node *tail;
	Node *curr;
	int numNodes;
};


#endif
