#include "semaphor.h"
#include "KerSem.h"
#include <iostream.h>


extern volatile PCB* running;
//extern volatile int context_switch_on_demand;
extern volatile int dispatch_demand;

#define lock running->lockFlag++;\

#define unlock\
	--(running->lockFlag);\
	if(running->lockFlag == 0 && dispatch_demand){\
		dispatch();\
	}\


Semaphore::Semaphore (int init){
	lock
	myImpl = new KernelSem(init, this);
	unlock
}


Semaphore::~Semaphore (){
	lock
	delete myImpl;
	unlock
}

int Semaphore:: val () const{
	return myImpl->getVal();
}

int Semaphore:: wait (Time maxTimeToWait){
	lock
	int ret = 1;
	if (--(myImpl->val) < 0){
		ret = myImpl->block(maxTimeToWait);
		running->retValForSem = 1;
	}
	unlock
	return ret;
}

int Semaphore:: signal(int n){
	lock
	int ret = n;
	if (n == 0){
		ret = 0;
		if((myImpl->val)++ < 0){
			myImpl->deblock(1);
		}
	}else if (n > 0){
		ret = 0;
		int oldVal = myImpl->val;
		myImpl->val += n;
		if (oldVal < 0){
			ret = myImpl->deblock(n);
		}
	}
	unlock
	return ret;
}
