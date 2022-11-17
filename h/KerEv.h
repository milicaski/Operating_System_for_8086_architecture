#ifndef _KEREV_H_
#define _KEREV_H_

#include "PCB.h"
#include "event.h"


class KernelEv{
public:
	KernelEv(IVTNo ivtNo, Event *myEv);
	~KernelEv();
	void signal();
private:
	friend class Event;
	friend class IVTEntry;
	int val;
	IVTNo numEntry;
	PCB *owner;
	PCB *blocked;
	Event *myEv;
	IVTEntry *myIVTEntry;
};



#endif
