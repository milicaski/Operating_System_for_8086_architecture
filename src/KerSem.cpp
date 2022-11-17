#include "KerSem.h"
#include "ArraySem.h"
#include <iostream.h>


volatile ArraySem *semaphores = new ArraySem(10);
extern volatile PCB* running;
extern volatile int must_dispatch_demand;


KernelSem::KernelSem(int init, Semaphore *mySem){
	this->mySem = mySem;
	val = init;
	waitingOnSem = new ListSem();
	id = ((ArraySem*)semaphores)->add(this);
}

KernelSem::~KernelSem(){
	((ArraySem*)semaphores)->remove(id);
	delete waitingOnSem;
	mySem = 0;
}

int KernelSem:: getVal() const{
	return val;
}

ID KernelSem::getId(){
	return id;
}


int KernelSem::block(int maxTimeToWait){
	running->flag = PCB::BLOCKED;
	waitingOnSem->add((PCB*)running, maxTimeToWait);
	must_dispatch_demand = 1;
	dispatch();
	return running->retValForSem;
}

int KernelSem::deblock(int num){
	int numUnB = 0;
	ListSem::Node* tmp = waitingOnSem->getFirstNode();
	while(tmp && numUnB < num){
		PCB* newPCB = waitingOnSem->removeFirst();
		if(newPCB != 0){
			newPCB->flag = PCB::READY;
			newPCB->retValForSem = 1;
			PCB::put(newPCB);
		}
		numUnB++;
		tmp = waitingOnSem->getFirstNode();
	}
	return numUnB;
}


void KernelSem:: freeIfTime(){
	for (int i = 0; i < ((ArraySem*)semaphores)->getCapacity(); i++){
		KernelSem *tmp = ((ArraySem*)semaphores)->get(i);
		if(tmp != 0){
			ListSem::Node *tmpNode = tmp->waitingOnSem->getFirstNode();

			if (tmpNode && tmpNode->startWaitingTime != 0)
				--(tmpNode->waitingTime);

			while (tmpNode && tmpNode->startWaitingTime != 0 && tmpNode->waitingTime == 0){
				PCB* newPCB = tmp->waitingOnSem->removeFirst();
				if(newPCB != 0){//za svaki sluc, ne treba nego ajde
					newPCB->retValForSem = 0;
					newPCB->flag = PCB::READY;
					PCB::put(newPCB);
					tmp->val++;
				}
				tmpNode = tmp->waitingOnSem->getFirstNode();
			}
		}
	}
}
