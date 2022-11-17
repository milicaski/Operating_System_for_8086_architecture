#include <dos.h>
#include "PCB.h"
#include "Array.h"
#include "ArraySem.h"
#include "ArrayIVT.h"
#include <iostream.h>
#include "KerSem.h"
#include "event.h"


#define lock asm {\
		pushf;\
		cli;\
	}
#define unlock asm popf


typedef void interrupt (*pInterrupt)(...);

volatile int context_switch_on_demand = 0;
volatile int dispatch_demand = 0;
volatile int must_dispatch_demand = 0;
pInterrupt oldTimer;
extern volatile int count;
extern volatile PCB* running;
extern volatile Array *threads;
extern volatile ArrayIVTEntry *events;
extern volatile ArraySem *semaphores;
extern PCB *idle;
unsigned tmpSS;
unsigned tmpSP;
unsigned tmpBP;

void tick(){}//bez otvorene zagrade

void interrupt timer(...){//za mod ako dodajem neku metodu koja ce da bude kritivcna moracu da namestam must_dispatch_demand na 1 pre poziva
	if (!context_switch_on_demand && running->timeSlice != 0) {
		count--;
	}

	if (!context_switch_on_demand){
			KernelSem::freeIfTime();
	}

	if ((count == 0 && running->timeSlice != 0) || context_switch_on_demand) {
		if(running->lockFlag == 0 || must_dispatch_demand){
			if(running->lockFlag == 0){
				dispatch_demand = 0;

			}
			if (must_dispatch_demand)
				must_dispatch_demand = 0;

			asm {
				mov tmpSP, sp
				mov tmpSS, ss
				mov tmpBP, bp
			}

			running->sp = tmpSP;
			running->ss = tmpSS;
			running->bp= tmpBP;

			PCB::put((PCB*)running);
			running = PCB::get();

			tmpSP = running->sp;
			tmpSS = running->ss;
			tmpBP = running->bp;
			count = running->timeSlice;

			asm {
				mov sp, tmpSP
				mov ss, tmpSS
				mov bp, tmpBP
			}
			if (((PCB*)running)->numSignals() > 0){
				((PCB*)running)->processSignals();
			}

		}else {
			dispatch_demand = 1;
		}
	}


	if(!context_switch_on_demand) {
		tick();
		asm int 60h;
	}
	context_switch_on_demand = 0;
}


void inic() {
	lock
	oldTimer = getvect(8);
	setvect(0x60,oldTimer);
	setvect(8,timer);
	unlock
}

void restore() {
	lock
	setvect(8,oldTimer);
	unlock
}

int userMain (int argc, char* argv[]){}//bez otvorene zagrade

class UserMainThread: public Thread{
public:
	UserMainThread(int argc, char** argv);
	void run();
	int ret;
private:
	int argc;
	char** argv;
};

UserMainThread::UserMainThread(int argc, char** argv){
	this->argc  = argc;
	this->argv = argv;
	ret = 1;
}

void UserMainThread::run(){
	ret = userMain (argc, argv);
}

class MainThread: public Thread{
public:
	MainThread():Thread(16384, 2){

	}
};


int main(int argc, char* argv[]){
	MainThread *mainThread = new MainThread();

	Idle *idleThread = new Idle();
	idle = (PCB*)(((Array*)threads)->get(idleThread->getId()));


	inic();

	UserMainThread *userMainT = new UserMainThread(argc, argv);
	userMainT->start();

	userMainT->waitToComplete();
	int ret = userMainT->ret;
	delete userMainT;

	restore();

	delete threads;
	delete mainThread;
	delete semaphores;
	delete events;
	delete idleThread;
	idle = 0;
	return ret;
}
