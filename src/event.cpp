#include "event.h"
#include "KerEv.h"
#include <iostream.h>


extern volatile PCB* running;
extern volatile int must_dispatch_demand;

extern volatile int dispatch_demand;

#define lock running->lockFlag++;\

#define unlock\
	--(running->lockFlag);\
	if(running->lockFlag == 0 && dispatch_demand){\
		dispatch();\
	}\



Event::Event (IVTNo ivtNo){
	lock
	myImpl = new KernelEv(ivtNo, this);
	unlock
}

Event::~Event (){
	lock
	delete myImpl;
	unlock
}

void Event::wait (){
	lock
	if (((PCB*)running)->getId() == myImpl->owner->getId()){
		if (myImpl->val == 1){
			myImpl->val = 0;
		}else{
			myImpl->blocked = (PCB*)running;
			running->flag = PCB::BLOCKED;
			must_dispatch_demand = 1;
			dispatch();
		}
	}
	unlock
}

void Event::signal(){
	lock
	if(myImpl->blocked == 0){
		myImpl->val = 1;
	}else{
		myImpl->blocked->flag = PCB::READY;
		PCB::put(myImpl->blocked);
		myImpl->blocked = 0;
		must_dispatch_demand = 1;
		dispatch();
	}
	unlock
}

