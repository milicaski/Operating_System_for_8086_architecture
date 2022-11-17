#ifndef _PCB_H_
#define _PCB_H_

#include "list.h"
#include "ArrayFun.h"
#include "ListSig.h"
#include "thread.h"
class Array;

class PCB{
	public:
		enum State{NOTSTARTED, READY, BLOCKED, FINISHED};
		PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
		Thread* getMyThread();
		ID getId();
		static void put(PCB* p);
		static PCB* get();
		~PCB();
		static void wrapper();
		void unblock();
		int getWaitingFor();
		int numSignals();
		void processSignals();
		static void handlerSignalZero();
		unsigned ss;
		unsigned sp;
		unsigned bp;
		unsigned *stack;
		Time timeSlice;
		State flag;
		int retValForSem;
		ArrayFun* copyFunSignals();
		int lockFlag;
	private:
		friend class Thread;
		Thread *parent;
		Thread *myThread;
		ID id;
		int waitingFor;
		List *waitingForMe;
		ListSig *reqSignals;
		ArrayFun *funSignals;
		static int globalyBlocked[16];
};


class Idle: public Thread{
public:
	Idle();
	void run();
};

#endif
