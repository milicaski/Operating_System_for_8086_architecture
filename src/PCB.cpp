#include "PCB.h"
#include <dos.h>
#include "SCHEDULE.H"
#include "Array.h"
#include <iostream.h>
#include "thread.h"


volatile PCB* running = 0;
//extern volatile int context_switch_on_demand;
extern volatile int dispatch_demand;

#define lock running->lockFlag++;\

#define unlock\
	--(running->lockFlag);\
	if(running->lockFlag == 0 && dispatch_demand){\
		dispatch();\
	}\


volatile int count = 0;


volatile Array *threads = new Array(10);
PCB *idle = 0;
volatile int conditionIdle = 1;

int PCB::globalyBlocked[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread):myThread(thread){
	lock
	if(running == 0){
		this->timeSlice = 1;
		flag = READY;
		waitingFor = 0;
		id = ((Array*)threads)->add(this);
		waitingForMe = 0;
		stack = 0;
		retValForSem = 1;
		reqSignals = new ListSig();
		funSignals = new ArrayFun();
		funSignals->registerHandler(0, PCB::handlerSignalZero);
		running = this;
		count = this->timeSlice;
		parent = 0;
		lockFlag = 0;
	}else{
		flag = NOTSTARTED;
		this->timeSlice = timeSlice;
		waitingFor = 0;
		waitingForMe= new List();
		stackSize /= 2;
		stack = new unsigned [stackSize];
		stack[stackSize-1]=0x200;
		stack[stackSize-2] = FP_SEG(&wrapper);
		stack[stackSize-3] = FP_OFF(&wrapper);
		this->ss = FP_SEG(stack+stackSize-12);
		this->sp = FP_OFF(stack+stackSize-12);
		this->bp = this->sp;
		id = ((Array*)threads)->add(this);
		retValForSem = 1;
		lockFlag = 0;
		parent = running->myThread;
		reqSignals = new ListSig();

		funSignals = ((PCB*)running)->copyFunSignals();
	}
	unlock
}


PCB:: ~PCB(){
	lock
	myThread = 0;
	parent = 0;
	if (stack != 0)
		delete stack;
	if (waitingForMe != 0)
		delete waitingForMe;
	if(reqSignals != 0)
		delete reqSignals;
	if (funSignals != 0)
		delete funSignals;
	unlock
}

Thread* PCB:: getMyThread(){
	return this->myThread;
}

ID PCB::getId(){
	return this->id;
}

void PCB:: put(PCB *newPCB){
	if ((newPCB->flag != READY)||(newPCB == idle)) return;
	Scheduler::put(newPCB);
}

PCB* PCB:: get(){
	PCB* newPCB = (PCB*)Scheduler::get();
	if (newPCB == 0)
		newPCB = idle;
	return newPCB;
}


void PCB:: unblock(){
	lock
	if (waitingForMe != 0){
		for (waitingForMe->currAtFirst(); waitingForMe->isCurr(); waitingForMe->moveCurr()){
			PCB* curr = waitingForMe->getCurr();
			curr->waitingFor--;
			waitingForMe->removeCurr();
			if (curr->waitingFor == 0){
				curr->flag = READY;
				PCB::put(curr);
			}
		}
	}
	unlock
}

int PCB::getWaitingFor(){
	return waitingFor;
}

int PCB::numSignals(){
	return reqSignals->getNumSignals();
}

void PCB::processSignals(){
	int numProcessed = 0;
	int numReq =  reqSignals->getNumSignals();
	while (numProcessed != numReq){
		int signal = reqSignals->get();
		if (globalyBlocked[signal] == 0 && funSignals->arraySignal[signal].blocked == 0){
			reqSignals->remove();

			for(int i = 0; i < funSignals->arraySignal[signal].numHandlers; i++){
				funSignals->arraySignal[signal].arrayHandlers[i]();
			}
		}else{
			reqSignals->skip();
		}
		numProcessed++;
	}
}

ArrayFun* PCB::copyFunSignals(){
	ArrayFun *newFunSignals = new ArrayFun(this->funSignals);
	return newFunSignals;
}

void PCB::handlerSignalZero(){
	if (globalyBlocked[2] == 0 && running->funSignals->arraySignal[2].blocked == 0){
		for(int i = 0; i < running->funSignals->arraySignal[2].numHandlers; i++){
			running->funSignals->arraySignal[2].arrayHandlers[i]();
		}
	}
	if(running->parent != 0)
			running->parent->signal(1);
	((PCB*)running)->unblock();
	running->flag = FINISHED;
	dispatch();//ovde ne treba valjda jer se zove iz timera ili disptach a brisem i bit tu
}

void PCB:: wrapper(){
	running->myThread->run();

	running->myThread->signal(2);
	dispatch();
	if(running->parent != 0)
		running->parent->signal(1);
	((PCB*)running)->unblock();
	running->flag = FINISHED;
	dispatch();
}


Idle:: Idle():Thread(1024, 1){

}


void Idle:: run() {
	while (conditionIdle){
		/*lock
		cout<<"idle"<<endl;
		unlock*/
	}

}

