#include "PCB.h"
#include "Array.h"
#include <iostream.h>
#include "thread.h"

extern volatile int context_switch_on_demand;
extern volatile PCB* running;
extern volatile int must_dispatch_demand;

extern volatile int dispatch_demand;

#define lock running->lockFlag++;\

#define unlock\
	--(running->lockFlag);\
	if(running->lockFlag == 0 && dispatch_demand){\
		dispatch();\
	}\


#define lockR asm {\
		pushf;\
		cli;\
	}
#define unlockR asm popf

extern volatile Array *threads;


extern PCB *idle;
void interrupt timer(...);

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock
	if(stackSize >= 65536) stackSize = 65535;
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}


ID Thread:: getId(){
	return myPCB->getId();
}

Thread:: ~Thread(){
	lock
	waitToComplete();
	((Array*)threads)->remove(getId());
	delete myPCB;
	unlock
}


void Thread:: start(){
	lock
	if (myPCB->flag != PCB::NOTSTARTED) {
		unlock
		return;
	}
	myPCB->flag = PCB::READY;
	PCB::put(myPCB);
	unlock
}


void Thread:: waitToComplete(){
	lock
	if ((this->myPCB->flag != PCB::FINISHED) && (((PCB*)running)->getId() != this->myPCB->getId()) && this->myPCB != idle ){
		this->myPCB->waitingForMe->add((PCB*)running);
		running->flag = PCB::BLOCKED;
		running->waitingFor++;
		must_dispatch_demand = 1;
		dispatch();
	}
	unlock
}

ID Thread::getRunningId(){
	return ((PCB*)running)->getId();
}

Thread* Thread::getThreadById(ID id){
	lock
	PCB* tr = ((Array*)threads)->get(id);
	Thread *thr = 0;
	if (tr != 0)
		thr = tr->getMyThread();
	unlock
	return thr;
}


void Thread::signal(SignalId signal){
	lock
	 if (signal > 15){
		 unlock
		 return;
	 }
	myPCB->reqSignals->put(signal);
	unlock
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	lock
	 if (signal > 15 || handler == 0) {
		 unlock
		 return;
	 }
	myPCB->funSignals->registerHandler(signal, handler);
	unlock
}

void Thread::unregisterAllHandlers(SignalId id){
	lock
	 if (id > 15){
		 unlock
		 return;
	 }
	myPCB->funSignals->unregisterAllHandlers(id);
	unlock
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	lock
	if (id > 15 || hand1 == 0 || hand2 == 0){
		unlock
		return;
	}
	myPCB->funSignals->swap(id, hand1, hand2);
	unlock
}

void Thread::blockSignal(SignalId signal){
	lock
	 if (signal > 15){
		 unlock
		 return;
	 }
	myPCB->funSignals->blockSignal(signal);
	unlock
}

 void Thread::blockSignalGlobally(SignalId signal){
	 lock
	 if (signal > 15){
		 unlock
		 return;
	 }
	 PCB::globalyBlocked[signal] = 1;
	 unlock
 }

void Thread::unblockSignal(SignalId signal){
	lock
	 if (signal > 15){
		 unlock
		 return;
	 }
	myPCB->funSignals->unblockSignal(signal);
	unlock
}

void Thread::unblockSignalGlobally(SignalId signal){
	lock
	 if (signal > 15){
		 unlock
		 return;
	 }
	 PCB::globalyBlocked[signal] = 0;
	 unlock
}



void dispatch(){
	lockR
	context_switch_on_demand = 1;
	timer();
	unlockR
}
